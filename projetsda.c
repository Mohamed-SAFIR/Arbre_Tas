#include <stdio.h>
#include <stdlib.h>

/*
structure d'un arbre binaire
*/
typedef struct node_s{
  char symbol;
  int freq;
  struct node_s *fg;
  struct node_s *fd;
}node;



/*
structure d'un tas
*/
typedef struct tas_s{
  int m;   /* nombre maximum d'éléments */
  int n;    /*  nombre d'élémennts dans le tas  */
  node ** tab;
}tas;






/*#########################################################*/
/*
cette fonction prend en parametre un caractere et une frequence
et créee un noeud et initialise son symbol et sa frequence et renvoie un noeud
*/
node * creer_node(char symbol, int freq){
  node *nw=(node*)malloc(sizeof(node));
  nw->symbol=symbol;
  nw->freq=freq;
  nw->fg=NULL;
  nw->fd=NULL;
  return nw;
}







/*#########################################################*/
/*
cette fonction prend un entier en parametre et initialise un tas en allouant la memoire
et affecter l'entier passé en parametre comme taille maximum du tas
*/
tas * inic_tas(int max){
  tas *nw=(tas*)malloc(sizeof(tas));
  nw->tab=(node**)malloc(max*sizeof(node));
  nw->m=max;
  nw->n=0;
  return nw;
}









/*#########################################################*/
/*
cette fonction verifie si le tas passé en parametre et vide ou pas
elle renvoie 1 s'il est vide, 0 sinon
*/
int est_vide(tas * ta){
  if(ta->n==0){
    return 1;
  }
  return 0;
}








/*#########################################################*/
/*
cette fonction pernd en parametre un tas et un noeud
elle insère le noeud dans le tas en respectant l'ordre du tas
il s'agit d'un tas minimum, donc la frequence de la racine doit etre
plus petite ou egale à toute les autres frequences des racines dans
les autres positions du tableau
*/
void inserer_tas(tas * ta, node * nd){
  int i,j;
  node *temp;
  if(ta->n+1 <= ta->m){/*verification que le tas n'est pas plein */
    i=ta->n;/* i reçoit le nombre de l'element du tableau*/
    j=(i-1)/2; /* on initialise le j qui prend l'indice du père de i */
    ta->n++;
    ta->tab[i]=nd; /* on insère le noeud dans la derniere case du tableau */
    /* on fait une boucle while et verifie que tand que le père et plus grand
    que son fils on echange les noeuds */
    while( (i>0) && (ta->tab[i]->freq < ta->tab[j]->freq) ){
      temp=ta->tab[i];
      ta->tab[i]=ta->tab[j];
      ta->tab[j]=temp;
      i=j;
      j=(i-1)/2;
    }
  }
}









/*#########################################################*/
/*
cette fonction permet de saisir les caracteres et les symbols des noeuds
et elle renvoie un tas
*/
tas* saisie_alphabet(){
  int max,fre;
  tas* t;
  node *nd;
  char cara;
  printf("veuillez saisir le nombre de caracteres: ");
  scanf("%d",&max);
  t=inic_tas(max); /* on initialise le tas on lui envoyant en parametre le nombre
  maximum de la tailles du tas */
  for(int i=1;i<=max;i++){
    /* on entre les caracteres et les symbols et créee des noeuds et on les insère
    dans le tas en appellant les fonctions 'creer_node' et 'inserer_tas'  */
    printf("veuillez saisir le caractere no %d: ",i);
    scanf(" %c",&cara);
    printf("veuillez saisir la frequence no %d: ",i);
    scanf("%d",&fre);
    printf("\n");
    nd=creer_node(cara,fre);
    inserer_tas(t,nd);
  }
  return t;
}












/*#########################################################*/
/*
cette fonction sert a afficher les noeuds qui se trouvent dans le tas passé
en argument
*/
void afficher(tas* t){
  int nb=t->n;
  for(int i=0;i<nb;i++){
    printf("(%c : %d);\t",t->tab[i]->symbol,t->tab[i]->freq);
  }
  printf("\n");
}













/*#########################################################*/
/*
cette fonction prend en parametre un tas et permet de supprimer le premier noeud
et actualise le tas en respectant l'ordre "tas minimum"
et renvoie le noeud qui a été supprimé
*/
node * supprimer_tas(tas * t){
  node *sup=t->tab[0]; /* on inialise un noeud au premier noeud du tas  */
  t->tab[0]=t->tab[t->n-1]; /* on ramène le dernier élément du tas et on le place au debut */
  t->n--; /* on decremente le nombre d'element du tas */
  int i=0;
  int continuer=1;
  int k;
  /* on fera une boucle wile qui permet d'actualiser le tas */
  while( (i<t->n) && (continuer)){
    if(2*i+1 < t->n){ /* on verifie si le fils gauche de l'indice i se trouve bien dans le tas */
      /* on compare les frequences de fils gauche avec celle de fils droit ET on met l'indice du plus petit dans une variable k */
      if( t->tab[2*i+1]->freq  <=  t->tab[2*i+2]->freq){
        k=2*i+1;
      }else{
        k=2*i+2;
      }
    }else{
      k=i+1;
    }
    /* on compare la frequence de l'indice i avec celle de l'un de ses fils ET si cette derniere et plus petite de celle de son père on fait un echange */
    if(t->tab[i]->freq > t->tab[k]->freq ){
      node * tmp=t->tab[i];
      t->tab[i]=t->tab[k];
      t->tab[k]=tmp;
      i=k;
    }else {
      continuer =0;
    }
  }
  return sup;
}











/*#########################################################*/
/*
cette fonction prend en parametre un tas et renvoie un arbre qu'on construit a partir cette fonction
en supprimant les deux noeuds dont leurs frequences sont plus petite dans le tas
et en creant un noeud où sa frequence et la somme des frequences des noueds qu'on a supprimé
et ses fils droit et gauche sont les noeuds supprimé
derniere etape de la fonction et d'inserer le noeud créee dans le meme tas jusqu'a avoir un seul noeud qui est l'arbre
*/
node * creer_arbre(tas * t){
  int n=t->n;
  for(int i=1;i<n;i++){

    node *x=supprimer_tas(t);
    node *y=supprimer_tas(t);
    node *z=creer_node(' ',x->freq+y->freq);
    z->fg=x;
    z->fd=y;
    inserer_tas(t,z);
    //afficher(t);

  }
  return supprimer_tas(t);
}









/*#########################################################*/
/*
cette fonction affiche l'arbre dans l'ordre infixe
          ( FILS GAUCHE ; RACINE ; FILS DROIT )
*/
void imprimer_arbre(node *nd){
  if(nd!=NULL){
    imprimer_arbre(nd->fg);
    printf("  %d  ",nd->freq);
    imprimer_arbre(nd->fd);
  }
}








/*#########################################################*/
/*
cette fonction verifie si le noeud passé en argument s'il s'agit d'une feuille ou non
en verifiant ses fils droit et gauche
elle retourne 1 si le noeud est une feuille, 0 sinon
*/
int est_feuille(node* nd){
  if(nd->fd==NULL && nd->fg==NULL){
    return 1;
  }
  return 0;
}











/*#########################################################*/
/*
cette fonction permet d'afficher le chemin de la racine jusqu'au feuilles en codage binaire
il s'agit d'écrire un 0 s'il derige vers la gauche, et 1 s'il derige vers la droit
*/
void imprimer_codes(node* n, char * code, int i){

  if(n->fg != NULL){
    code[i]='0';
    imprimer_codes(n->fg,code,i+1);
  }
  if(n->fd != NULL){
    code[i]='1';
    imprimer_codes(n->fd,code,i+1);
  }

  if( (est_feuille(n))==1){
    code[i]='\0';
    printf("(%c = %s)\n",n->symbol,code);
  }
}










/*#########################################################*/
/*
cette fonction prend en argument un noeud et supprime tout l'arbre en commençant des feuilles et on remonte
*/
void supprimer_arbre(node *n){
  if(n !=NULL){
    supprimer_arbre(n->fg);
    supprimer_arbre(n->fd);
    free(n);
  }
}









/*#########################################################*/
/*
cette fonction prend en argument un tas et désalloue la mémoire alloué au tableau du tas
et au tas en lui même.
*/
void liberer_memoire_tas(tas *t){
  for(int i=0; i<t->n;i++){
    supprimer_arbre(t->tab[i]);
  }
  free(t);
}









/*#########################################################*/
int main(){

  /* on fait appelle a la fonction 'saisie_alphabet' pour créer unn tas */
  tas * new=saisie_alphabet();
  printf("affichage du tas initial \n");
  afficher(new);


  /* on créee un arbre à parir de tas qu'on a créer, et on l'affecte a un noeud nd1 */
  node *nd1=creer_arbre(new);
  printf("affichage de l'arbre dans l'ordre infixe \n");
  imprimer_arbre(nd1);
  printf("\n\n\n");

  char car[100];
  printf("le codage est: \n");
  imprimer_codes(nd1,car,0);

  liberer_memoire_tas(new);
  return 0;
}
