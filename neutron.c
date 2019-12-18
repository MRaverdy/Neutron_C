#include <stdio.h>

/*Codes couleurs utilsés pour l'affichage du damier.*/
#define reset "\033[0m"
#define reverse "\033[7m"
#define crouge "\033[;31m"
#define cbleu "\033[;34m"
#define cvert "\033[;32m"

/*L correspond à la longueur et à la largeur du damier.*/
#define L 5

/*vecx et vecy sont les vecteurs de déplacement. Ils contiennent les coefficients des 8 deplacements. Par exemple, vecx[0] et vecy[0] correspondent au deplacement 1.*/
int vecx[]={-1,-1,0,1,1,1,0,-1};
int vecy[]={0,1,1,1,0,-1,-1,-1};

/*Le type Color indique la couleur présente dans une case.*/
enum Color{
    rouge,
    vert,
    bleu,
    vide,
};

/*Structure représentant la case.*/
typedef struct{
    int oqp;/*Variable qui indique si la case est occupée ou non.*/
    enum Color couleur;/*Couleur présente dans la case.*/
} Case;

/*Structure représentant le damier.*/
typedef struct{
    Case C[L][L];/*Matrice L*L qui représente le damier.*/
    int posN[2];/*posN est un vecteur de dimension 2 qui contient les coordonnées x et y du neutron.*/
} Damier;
    

void initialisation(Damier *D){
    /*On commence par remplir la 1ère ligne par des pions rouges.*/
    for (int k=0;k<L;k++){
        D->C[0][k].oqp=1;
        D->C[0][k].couleur=rouge;
    }
    /*On remplit le damier par des cases vides sauf la 1ère et la dernière ligne.*/
    for (int j=1;j<L-1;j++){
        for (int i=0;i<L;i++){
        D->C[j][i].oqp=0;
        D->C[j][i].couleur=vide;
        }
    }
    /*On positionne le neutron dans la case centrale du damier.*/
    D->C[(L-1)/2][(L-1)/2].oqp=1;
    D->C[(L-1)/2][(L-1)/2].couleur=vert;
    /*On indique la position du neutron dans posN.*/
    D->posN[0]=(L-1)/2;
    D->posN[1]=(L-1)/2;
    /*Pour finir, on remplit la dernière ligne par des pions bleus.*/
    for (int k=0;k<L;k++){
        D->C[L-1][k].oqp= 1;
        D->C[L-1][k].couleur=bleu;
    }
}


 void affichage(Damier D){/*Fonction qui affiche le damier.*/
    /*On modélise un damier avec des "-" et des "|" en fonction de L.*/
    printf("  ");
    for (int n=0;n<L;n++){
            printf("---");}
    printf("-\n");
    for (int j=0;j<L;j++){
        printf("%d |",L-j);
        for (int i=0;i<L;i++){
            if (D.C[j][i].oqp){
                /*On fait correspondre la couleur de la case (type enum color) avec son code couleur.*/
                if (D.C[j][i].couleur==rouge){
                printf("%s%s %s%s %s",crouge,reverse,crouge,reverse,reset);
                }
                if (D.C[j][i].couleur==vert){
                printf("%s%s %s%s %s",cvert,reverse,cvert,reverse,reset);
                }
                if (D.C[j][i].couleur==bleu){
                printf("%s%s %s%s %s",cbleu,reverse,cbleu,reverse,reset);
                }
            }
            else{
                printf("  ");}
            printf("|");}
        printf("\n  ");
        for (int m=0;m<L;m++){
            printf("---");}
        printf("-\n");}
    printf("   ");
    for (int k=1;k<=L;k++){
        printf("%d  ",k);}
    printf("\n");
 }
  
void deplace(int x, int y, Damier *D){/*Cette fonction effectue le déplacement d'un pion.*/
    /*x et y sont les coordonnées du pion que l'on veut déplacer. 0<=x<=L-1 et 0<=y<=L-1.*/
    int x1=x;
    int y1=y;
    /*On utlise deux variables locales x1 et y1 pour transposer le pion dans sa nouvelle case.*/
    enum Color colors;/*Variable de transposition de la couleur de la case.*/
    int dir;/*Variable correspondant à la direction.*/
    while ((x==x1) && (y==y1)){/*le while permet de rechoisir une direction si la pion n'a pas bougé.*/
        printf("Choisissez une direction entre 1 et 8 : ");
        scanf("%d",&dir);
        do {
            x1+=vecx[dir-1];
            y1+=vecy[dir-1];}
        while((x1>=0) && (x1<=L-1) && (y1>=0) && (y1<=L-1) && (D->C[x1][y1].oqp==0));/*Tant que x1 et y1 sont dans les dimensions du damier et que la case correspondante est libre, on regarde la case suivante en fonction de la direction choisie.*/
        x1-=vecx[dir-1];
        y1-=vecy[dir-1];}/*On revient en arrière d'une case après la boucle car celle-ci effectue une incrémentation de trop pour vérifier les conditions.*/
    colors=D->C[x][y].couleur;
    D->C[x][y].oqp=0;/*La case initiale n'est plus occupée.*/
    D->C[x][y].couleur=vide;
    D->C[x1][y1].oqp=1;/*La case finale est maintenant occupée.*/
    D->C[x1][y1].couleur=colors;/*On transpose les deux couleurs de la case initiale et de la case finale.*/
    if (D->C[x1][y1].couleur==vert){/*Si le pion déplacé était le neutron, on range ses nouvelles coordonnées dans posN.*/
        D->posN[0]=x1;
        D->posN[1]=y1;
    }
}

int verif_bloque(int x, int y, Damier D){/*Fonction qui vérifie si le pion dont les coordonnées sont passées en paramètre peut être déplacé ou non. Cette fonction renvoie 1 si le pion est bloqué et 0 sinon.*/
    int i=0,bloque=1,x1,y1;
    while ((i<8) && (bloque)){
        x1=x+vecx[i];
        y1=y+vecy[i];/*x1 et y1 sont deux variables locales qui permettent de tester si les cases autour du pion sont occupées ou non. */
        if ((x1>=0) && (x1<=L-1) && (y1>=0) && (y1<=L-1)){/*On vérifie que la case testée soit bien dans les dimensions du damier.*/
            if (D.C[x1][y1].oqp==0){/*Dès qu'une case adjacente est disponible, la variable bloque passe a 0.*/
                bloque=0;}}
        i++;}
    return bloque;}
    
void verif_tous_en_face(int *nonfin, enum Color couleurtour, Damier D){/*Fonction qui vérifie si un joueur a réussi à placer tous ses pions dans le camp adverse.*/
/*nonfin est une variable qui fixe si la partie doit continuer ou pas.*/
    int c=0;
    if (couleurtour==rouge){
        *nonfin=0;
        while ((c<L) && (*nonfin==0)){
            if (D.C[L-1][c].couleur!=couleurtour){/*Dès qu'une case de la ligne n'est pas occupée par un pion de la couleur choisie, nonfin passe à 1.*/
                *nonfin=1;
            }
        c++;
        }
    }
    else if (couleurtour==bleu){
        *nonfin=0;
        while ((c<L) && (*nonfin==0)){
            if (D.C[0][c].couleur!=couleurtour){
                *nonfin=1;
            }
        c++;
        }
    }
}

void verif_pions_bloques(int *nonfin, enum Color couleurtour, Damier D){/*Fonction qui vérifie si un joueur peut bouger au moins un de ses pions ou non.*/
    int nbpionimmobile=0,nbpion=0,j=0,k;/*nbpionimmobile compte le nombre de pions que le joueur ne peut pas déplacer. nbpion compte le nombre de pions dont on a vérifié la possibilité ou non de déplacement, pour permettre ainsi d'arreter la boucle plus tot lorsque nbpion=L.*/
    while ((j<L) && (nbpion<L) && (*nonfin)){
        k=0;
        while ((k<L) && (nbpion<L) && (*nonfin)){                
            if (D.C[j][k].couleur==couleurtour) {
                nbpionimmobile+=verif_bloque(j,k,D);
                nbpion++;}
            k++;
            }
        j++;
        if (nbpionimmobile==L){/*Lorsque le nombre de pions non déplacables est égal au nombre total de pion, nonfin passe à 0.*/
            *nonfin=0;
        }
    }
}
    
void verif_neutron_dans_camp(int *nonfin, enum Color *couleurtour,Damier D){/*Fonction qui vérifie si un joueur a réussi à placer le neutron dans le camp adverse.*/
    int f=0;
    if (D.posN[0]==0){
        *couleurtour=rouge;
        while ((f<L) && (*couleurtour==rouge)){/*Ici on vérifie le cas ou toutes les cases de la ligne d'un joueur sont occupées par quatre pions adverses et le neutron. Dans ce cas, le joueur possédant le neutron et les pions adverses dans son camp perd.*/
            if ((D.C[0][f].couleur!=bleu) && (D.C[0][f].couleur!=vert)){
                *couleurtour=bleu;
            }
            f++;
        }
        *nonfin=0;
    }
    else if (D.posN[0]==L-1){
        *couleurtour=bleu;
        while ((f<L) && (*couleurtour==bleu)){
            if ((D.C[L-1][f].couleur!=rouge) && (D.C[L-1][f].couleur!=vert)){
                *couleurtour=rouge;
            }
            f++;
        }
        *nonfin=0;
    }
}

void tour(int *i, int *nonfin, enum Color *couleurtour, Damier *D){
    int x,y;
    do {
        printf("Choisissez les coordonnées x,y du pion à déplacer : ");
        scanf("%d,%d",&x,&y);}
    while (D->C[L-x][y-1].couleur!=*couleurtour);/*Cette ligne permet au joueur de choisir un nouveau pion si il a choisi un pion adverse ou une case vide.*/
    deplace(L-x,y-1,D);
    affichage(*D);
    
    if (*i%2==0){/*Cela nous permet d'alterner entre les joueurs rouge et bleu à chaque tour.*/
        printf("Joueur rouge :\n");
        *couleurtour=rouge;
    }
    else{
        printf("Joueur bleu :\n");
        *couleurtour=bleu;
    }
    
    verif_tous_en_face(nonfin,*couleurtour,*D);/*On vérifie si le joueur a placé tous ses pions en face.*/
    
    if (verif_bloque(D->posN[0],D->posN[1],*D)){/*On vérifie que le neutron ne soit pas bloqué.*/
        *nonfin=0;}
    
    if (*nonfin){
        
        printf("Déplacez le neutron.\n");
        deplace(D->posN[0],D->posN[1],D);
        affichage(*D);
        
        verif_pions_bloques(nonfin,*couleurtour,*D);/*On vérifie si le joueur peut bouger au moins un pion.*/
        
        verif_neutron_dans_camp(nonfin,couleurtour,*D);/*On vérifie que le neutron ne soit pas dans un des deux camps.*/
        
        *i=*i+1;
    }
}

int main(){
    Damier D;
    enum Color couleurtour=bleu;/*couleurtour détermine le joueur qui est actuellement en train de jouer. Elle nous sert également à contenir le joueur gagnant à la fin du programme.*/
    int i=0,nonfin=1;/*nonfin est une variable qui détermine si la partie est finie ou non. si nonfin=1, la partie n'est pas finie et si nonfin=0, la partie est terminée.*/
    initialisation(&D);
    affichage(D);
    printf("Joueur bleu :\n");
    while (nonfin){
    tour(&i,&nonfin,&couleurtour,&D);
    }
    if (couleurtour==rouge){
        printf("-------------------\nLes bleus gagnent !\n-------------------\n");
    }
    else if (couleurtour==bleu){
        printf("-------------------\nLes rouges gagnent !\n-------------------\n");
    }
        
    return 0;
}
