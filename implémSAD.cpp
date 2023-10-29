
#ifndef ABR_CPP
#define ABR_CPP
 
#include "implémSAD.h"
#include "implémSAD.cpp"

// Constructeur
template <class T>
ABR <T> :: ABR():racine (0), compteur (0){}
 
// Destructeur
template <class T>
ABR <T> :: ~ABR ()
{
    detruire(racine);
}
 
template <class T>
Noeud <T>* ABR <T> :: CreerNoeud (const T& valeur){
    Noeud <T>* temp = new Noeud <T>;
    temp -> donnees = valeur;
    temp -> gauche = NULL;
    temp -> droit = NULL;
    return temp;
}
 
// Inserer un noeud
template <class T>
void ABR <T> :: inserer (const T& valeur)
{
    inserer (valeur, racine);
    compteur++;
}
 
// Rechercher un noeud
template <class T>
Noeud <T>* ABR <T> :: recherche (const T& valeur) const
{
    Noeud <T>* parent = 0;  
    return recherche (valeur, racine);
}
 
// Parcours infixe
template <class T>
void ABR <T> :: infixe () const
{
    infixe(racine);
}
 
// Parcours prefixe
template <class T>
void ABR <T> :: prefixe () const
{
    prefixe(racine);
}
 
// Parcours postfixe
template <class T>
void ABR <T> :: postfixe () const
{
    postfixe(racine);
}
 
// Nombre de noeud dans l'arbre
template <class T>
int ABR <T> :: taille () const
{
    return compteur;
}
 
template <class T>
bool ABR <T> :: estVide () const
{
    return (compteur == 0);
}
 
// La fonction d'aide (Récursive) appelée par le destructeur
template <class T>
void ABR <T> :: detruire (Noeud <T>* ptr)
{
    if (!ptr){return;}
     
    detruire (ptr -> gauche); // détruire le sous-arbre gauche
    detruire (ptr -> droit); // détruire le sous-arbre droit
    delete ptr; // détruire la racine
}
 
// LA fonction d'aide (Récursive) appelée par la fonction membre inserer
template <class T>
void ABR <T> :: inserer (const T& valeur, Noeud <T>*& ptr)
{
    // si l'arbre vide, inserer comme racine
    if (!ptr){
        ptr = CreerNoeud (valeur);
        return;
    }
    // si la valeur est inférieure à la valeur de racine, 
    // insérer le noeud dans le sous-arber gauche
    else if (valeur < ptr -> donnees){
        inserer (valeur, ptr -> gauche);
    }
    // Sinon, insérer le noeud dans le sous-arber droit
    else{
        inserer (valeur, ptr -> droit);
    }
}
 
// Fonction d'aide (Récursive) appeler par la fonction infixe
template <class T>
void ABR <T> :: infixe (Noeud <T>* ptr) const
{
    if (!ptr){return;}
     
    infixe (ptr -> gauche);
    cout << ptr -> donnees << '\t';
    infixe (ptr -> droit);
}
 
// Fonction d'aide (Récursive) appeler par la fonction prefixe
template <class T>
void ABR <T> :: prefixe (Noeud <T>* ptr) const
{
    if (!ptr){return;}
     
    cout << ptr -> donnees << '\t';
    prefixe (ptr -> gauche);
    prefixe (ptr -> droit);
}
 
// Fonction d'aide (Récursive) appeler par la fonction postfixe
template <class T>
void ABR <T> :: postfixe (Noeud <T>* ptr) const
{
    if (!ptr){return;}
     
    postfixe (ptr -> gauche);
    postfixe (ptr -> droit);
    cout << ptr -> donnees << '\t';
}
 
// Fonction d'aide (Récursive) appeler par la fonction membre recherche
template <class T>
Noeud <T>* ABR <T> :: recherche (const T& valeur, Noeud <T>* ptr, Noeud <T>*& parent) const
{
    if (!ptr){
        //Arbre est vide
        return NULL;
    }
    else if ((ptr->donnees) == valeur){
        // la valeur recherchée est stockée dans la racine
        return ptr;
    }
    else if (valeur < (ptr->donnees)){
        parent=ptr;
        // la valeur recherchée est dans le sous-arbre gauche
        return recherche (valeur, ptr->gauche, parent);
    }
    else{
        parent=ptr;
        // sinon, la valeur recherchée est dans le sous-arbre droit
        return recherche (valeur, ptr->droit, parent);
    }
}
 
template <class T>
Noeud <T>* ABR <T> :: successeur (Noeud <T>* ptr, Noeud <T>*& parent) const{
    if (!ptr){return NULL;}
 
    Noeud <T>* courant = ptr;
    while (courant->gauche != 0){
        parent = courant;
        courant = courant -> gauche;
 
    }
    return courant;
}
 
template <class T>
Noeud <T>* ABR <T> :: predecesseur (Noeud <T>* ptr, Noeud <T>*& parent) const{
    if (!ptr){return NULL;}
 
    Noeud <T>* courant = ptr;
    while (courant->droit != 0){
        parent = courant;
        courant = courant -> droit;
    }
    return courant;
}
 
 
template <class T>
void ABR <T> :: supprimer (const T& valeur)
{
    Noeud <T>* parent = 0;  
    Noeud <T>* del = recherche(valeur, racine, parent);
    if(del == 0){
        cout << "Le noeud n'appartient pas a l'arbre" << endl;
    }
    else{
        supprimer(del, parent);
    }
}
 
 
// Fonction d'aide (Récursive) appeler par la fonction membre supprimer
template <class T>
void ABR <T> :: supprimer (Noeud <T>* ptr, Noeud <T>* parent){
     
    if (ptr->gauche == 0 && ptr->droit == 0)  
    {
        cout << ptr->donnees << parent->donnees<< endl;
        if (ptr != racine)  
        {  
            if (parent->gauche == ptr)  
                parent->gauche = NULL;  
            else 
                parent->droit = NULL;  
        }  
        else 
            racine = NULL;  
   
        delete ptr;     
    } 
    else if (ptr->gauche && ptr->droit)  
    {
        Noeud <T>* pere = ptr;
        // ici vous pouvez utiliser le prédécesseur aussi  
        Noeud <T>* succ  = successeur(ptr->droit, pere);  
        int val = succ->donnees;  
        supprimer(succ, pere);  
        ptr->donnees = val;  
    } 
    else 
    {  
        Noeud <T>* enfant = (ptr->gauche)? ptr->gauche: ptr->droit;  
        if (ptr != racine)  
        {  
            if (ptr == parent->gauche)  
                parent->gauche = enfant;  
            else 
                parent->droit = enfant;  
        }  
   
        else 
            racine = enfant;  
 
        delete ptr;
    }    
}
 
 
int main ( ){
 
    // Instanciation d'un ABR

    ABR arbre;
    arbre.inserer("nb1");
    arbre.inserer("nb0_dist_to_nb1");
    arbre.inserer("uts0");
    arbre.inserer("uts1_dist_to_uts0");
    arbre.inserer("ts_switch");
    arbre.inserer("nb_switch");
    arbre.inserer("matrix_size_n");
    arbre.inserer("matrix_size_m");
    arbre.inserer("fts0");
    arbre.inserer("fts1_dist-to_fts0");
    arbre.inserer("performance");
    
    cout << '\n';
    cout << "Le parcours infixe de cet arbre est : "<< endl;
    arbre.infixe();
    cout << '\n';
 
    cout << '\n';
    cout << "Le parcours postfixe de cet arbre est : "<< endl;
    arbre.postfixe();
    cout << '\n';
 
    cout << '\n';
    cout << "Le parcours prefixe de cet arbre est : "<< endl;
    arbre.prefixe();
    cout << '\n';
 
    cout << '\n';
    
    return 0;
}



#endif
