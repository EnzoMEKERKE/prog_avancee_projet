// Inclusion des bibliothèques nécessaires
#include <iostream>
#include <thread>
#include <mutex>
#include <deque>
#include <chrono>
#include <semaphore.h>
#include <cstdlib>

using namespace std;

// Variables globales
mutex print_mutex;  // Mutex pour synchroniser l'affichage
mutex mtx;  // Mutex pour protéger l'accès à la salle d'attente et au client actuel
sem_t caissierLibre;  // Sémaphore indiquant si le caissier est libre
sem_t clientsEnAttente;  // Sémaphore comptant le nombre de clients en attente
const int NB_CHAISES = 5;  // Nombre de chaises dans la zone d'attente prioritaire
deque<int> salleAttente;  // Liste des clients en attente
int clientActuel = -1;  // ID du client actuellement servi, -1 = aucun

// Fonction exécutée par le thread du caissier
void caissier() {
    while (true) {
        sem_wait(&clientsEnAttente);  // Attendre qu'un client arrive
        mtx.lock();  // Verrouiller le mutex pour accéder aux données partagées

        // Déterminer quel client sera servi
        int client;
        if (clientActuel != -1) {
            client = clientActuel;
            clientActuel = -1;
        } else {
            // Choisir un client au hasard dans la salle d'attente
            int randomIndex = rand() % salleAttente.size();
            client = salleAttente[randomIndex];
            salleAttente.erase(salleAttente.begin() + randomIndex);
        }

        // Informer que le caissier sert ce client
        print_mutex.lock();
        cout << "Le caissier sert le client " << client << endl;
        print_mutex.unlock();

        mtx.unlock();
        this_thread::sleep_for(chrono::seconds(4));  // Simule la durée du service

        // Informer que le caissier a terminé de servir le client
        print_mutex.lock();
        cout << "Le caissier a termine de servir le client " << client << endl;
        print_mutex.unlock();

        sem_post(&caissierLibre);  // Indiquer que le caissier est maintenant libre
    }
}

// Fonction exécutée par chaque thread client
void client(int id) {
    mtx.lock();  // Verrouiller le mutex pour accéder aux données partagées

    // Essayer d'accéder directement au caissier
    if (sem_trywait(&caissierLibre) == 0) {
        clientActuel = id;
        print_mutex.lock();
        cout << "Client " << id << " reveille le caissier et est servi directement a la caisse prioritaire." << endl;
        print_mutex.unlock();

        sem_post(&clientsEnAttente);
        mtx.unlock();
        sem_wait(&caissierLibre);
    } else if (salleAttente.size() < NB_CHAISES) {  // Si le caissier est occupé, essayer de s'asseoir dans la salle d'attente
        salleAttente.push_back(id);
        print_mutex.lock();
        cout << "Client " << id << " attend dans la zone d'attente prioritaire. Clients en attente : " << salleAttente.size() << endl;
        print_mutex.unlock();

        mtx.unlock();
        sem_post(&clientsEnAttente);
        sem_wait(&caissierLibre);
    } else {  // Si la salle d'attente est pleine, le client part
        print_mutex.lock();
        cout << "Client " << id << " part car la zone d'attente est pleine." << endl;
        print_mutex.unlock();
        mtx.unlock();
    }
}

int main() {
    srand(time(NULL));  // Initialiser le générateur de nombres aléatoires
    sem_init(&caissierLibre, 0, 1);  // Initialiser le sémaphore du caissier comme libre
    sem_init(&clientsEnAttente, 0, 0);  // Initialiser le sémaphore des clients en attente à 0

    thread caissierThread(caissier);  // Lancer le thread du caissier
    this_thread::sleep_for(chrono::seconds(1));

    print_mutex.lock();
    cout << "Le caissier se repose car personne n'attend à la caisse prioritaire." << endl;
    print_mutex.unlock();

    thread clients[10];  // Créer 10 threads clients
    for (int i = 0; i < 10; i++) {
        clients[i] = thread(client, i);
        this_thread::sleep_for(chrono::seconds(1));
    }

    for (int i = 0; i < 10; i++) {
        clients[i].join();  // Attendre que tous les threads clients se terminent
    }

    caissierThread.detach();  // Détacher le thread du caissier pour qu'il continue à s'exécuter indépendamment

    sem_destroy(&caissierLibre);  // Détruire le sémaphore du caissier
    sem_destroy(&clientsEnAttente);  // Détruire le sémaphore des clients en attente

    return 0;
}
