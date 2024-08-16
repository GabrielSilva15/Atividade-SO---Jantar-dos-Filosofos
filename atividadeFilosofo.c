#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define NUM_FILOSOFOS 5

// Semaforos a serem utilizados para os hashis
sem_t semaforosHashis[NUM_FILOSOFOS];
// Semaforo usado para limitar a quantidade de filosofos comendo ao mesmo tempo
sem_t semaforo;

void* filosofo(void* num) {
    int idFilosofo = *((int*)num);
    int hashiEsquerdo = idFilosofo;
    int hashiDireito = (idFilosofo + 1) % NUM_FILOSOFOS;

    while (1) {
        // Simulando a ação de pensar do filosofo
        printf("Filósofo %d está pensando.\n", idFilosofo);
        
        sleep(rand() % 5 + 1); //Determinando tempo que o filosofo usa para pensar entre 1 a 5 segundos 

        // Verifica se o hashis esta disponível para uso
        sem_wait(&semaforo); 
        sem_wait(&semaforosHashis[hashiEsquerdo]); 
        sem_wait(&semaforosHashis[hashiDireito]); 

        // Informando que o filosofo esta comendo
        printf("Filósofo %d está comendo.\n", idFilosofo);
    
        sleep(rand() % 5 + 1);  //Determinando tempo que o filosofo usa para comer entre 1 a 5 segundos 
		
		printf("Filósofo %d terminou de comer.\n", idFilosofo); //Informando que o filosofo terminou de comer
		
        // Deixa os hashis disponiveis para uso para os filosofos
        sem_post(&semaforosHashis[hashiEsquerdo]); 
        sem_post(&semaforosHashis[hashiDireito]); 
        sem_post(&semaforo); 
    }
}

int main() {
    pthread_t threads[NUM_FILOSOFOS];
    int idsFilosofos[NUM_FILOSOFOS];
    srand(time(NULL));


    int i;
    for (i = 0; i < NUM_FILOSOFOS; i++) {
        sem_init(&semaforosHashis[i], 0, 1); 
    }
    sem_init(&semaforo, 0, semaforosHashis - 1);


    int j;
    for (j = 0; j < NUM_FILOSOFOS; j++) {
        idsFilosofos[j] = j;
        pthread_create(&threads[j], NULL, filosofo, &idsFilosofos[j]);
    }

    
    int k;
    for (k = 0; k < NUM_FILOSOFOS; k++) {
        pthread_join(threads[k], NULL);
    }

   
    int l;
    for (l = 0; l < NUM_FILOSOFOS; l++) {
        sem_destroy(&semaforosHashis[l]);
    }
    sem_destroy(&semaforo);

    return 0;
}




// Explicação:
/*

O mecanismo utilizado foi o de semáforos, ele foi utilizado para a representação dos hashis e para limitar a quantidade de filosofos
que podem pegar os hashis e comer ao mesmo tempo, nesse mecanismo foi utilizado uma constante global que pega o numero de filosofos que subtraido por um
garante evitar que aconteça deadlocks. Uma função para representar os filosofos foi criada e ela foi representada por uma thread em que pode pensar e comer,
a cada ação o usuario informa se esta pensando, comendo e se terminou de comer. Cada filosofo vai ter seu momento de comer de acordo se os hashis a sua direita e esquerda
estiverem disponiveis e ao terminar ele vai estar disponibilizando esses hashis. No arquivo main é inicializado os semáforos e criado suas threads parar cada filosofo.

*/
