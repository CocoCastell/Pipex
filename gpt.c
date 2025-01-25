#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pid;
    int fd[2];

    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }
    pid = fork();
    if (pid == 0) {
        int pid2;
        int fd2[2];
        pipe(fd2);

        if (pid2 != 0) {
            // Premier enfant (après avoir forké le second enfant)
            int nb2;

            close(fd[1]);  // Fermer l'écriture du premier pipe
            close(fd2[0]); // Fermer la lecture du deuxième pipe

            // Lire la valeur du premier pipe
            read(fd[0], &nb2, sizeof(nb2));
            close(fd[0]); // Fermer après lecture

            nb2 += 6; // Ajouter 6
            write(fd2[1], &nb2, sizeof(nb2));
            close(fd2[1]); // Fermer après écriture

            waitpid(pid2, NULL, 0); // Attendre le second enfant
        } else {
            // Deuxième enfant
            int nb3;

            close(fd[0]);  // Fermer le premier pipe
            close(fd2[1]); // Fermer l'écriture du deuxième pipe

            // Lire la valeur du deuxième pipe
            read(fd2[0], &nb3, sizeof(nb3));
            close(fd2[0]); // Fermer après lecture

            nb3 += 5; // Ajouter 5
            write(fd[1], &nb3, sizeof(nb3));
            close(fd[1]); // Fermer après écriture
        }
    } else {
        // Parent
        int nb = 4;
        int new_nb;

        write(fd[1], &nb, sizeof(nb));
        close(fd[1]); // Fermer l'écriture après utilisation

        waitpid(pid, NULL, 0); // Attendre le premier enfant

        // Lire la valeur finale depuis le premier pipe
        read(fd[0], &new_nb, sizeof(new_nb)); // Lecture effectuée ici
        close(fd[0]); // Fermer après lecture

        // Afficher la valeur finale
        printf("Résultat final : %d\n", new_nb);
    }

    return 0;
}

