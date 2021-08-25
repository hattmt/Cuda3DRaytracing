

#ifndef LIB_H
#define LIB_H

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
/**
*		
*		
*	\authors Attimont Henri 
*	\brief Librairie contenant les méthodes pour le traitement de vecteurs 3D
*/

/**
*	\union vec3 
*	\brief Vecteur 3D
*/
typedef union
{
	struct{
		double x;/*!<coordonnée x du vecteur 3D */
		double y;/*!<coordonnée y du vecteur 3D*/
		double z;/*!<coordonnée z du vecteur 3D*/
	};
	
	double data[3];/*!< accès aux variables de la structure sous la forme d'un tableau. */

}vec3;



vec3 vec3create_(double x, double y, double z);

 __device__ vec3 vec3create(double x, double y, double z);

/**
*	\fn vec3 add3(vec3 v1, vec3 v2)
*	\brief Additionne les 2 vecteurs 3D v1 et v2
*	\param v1 Premier vecteur 3D type vec3
*	\param v2 Deuxième vecteur 3D type vec3
*	\return Retourne la somme de v1 et v2, vecteur de type vec3
*/
 __device__ vec3 add3(vec3 v1, vec3 v2);
 vec3 add3_(vec3 v1, vec3 v2);

/**
*	\fn vec3 sub3(vec3 v1, vec3 v2)
*	\brief Réalise la soustraction de deux vecteurs
*	\param v1 Premier vecteur 3D type vec3
*	\param v2 Deuxieme vecteur 3D type vec3
*	\return Retourne un vecteur de type vec3 qui est la différence de v1 et v2 
*/
 __device__ vec3 sub3(vec3 v1, vec3 v2);
 vec3 sub3_(vec3 v1, vec3 v2);
/**
*	\fn vec3 mul3(vec3 v1, double nmbr)
*	\brief Multiplie les coordonées du vecteur v1 par nmbr
*	\param v1 Vecteur 3D type vec3
*	\param nmbr Flottant
*	\return Retourne la multiplication de v1 et nmbr en vec3
*/
 __device__ vec3 mul3(vec3 v1, double nmbr);
vec3 mul3_(vec3 v1, double nmbr);
 __device__ vec3 mul3_vec(vec3 v1, vec3 v2);
/**
*	\fn vec3 div3(vec3 v1, double nmbr)
*	\brief Divise les coordonées du vecteur v1 par nmbr
*	\param v1 Vecteur 3D type vec3
*	\param nmbr Flottant
*	\return Retourne un vecteur vec3 qui est la division de v1 par nmbr
*/
 __device__ vec3 div3(vec3 v1, double nmbr);
vec3 div3_(vec3 v1, double nmbr);
/**
*	\fn double norm3(vec3 v1)
*	\brief Renvoie la norme du vecteur v1
*	\param v1 Vecteur 3D type vec3
*	\return Retourne la norme de v1 en double
*/
 __device__ double norm3(vec3 v1);

/**
*	\fn vec3 normalize3(vec3 v1)
*	\brief Renvoie le vecteur normalisé du vecteur v1
*	\param v1 Vecteur 3D type vec3
*	\return Retourne un vecteur vec3 qui est la normalisation de v1
*/
 __device__ vec3 normalize3(vec3 v1);
vec3 normalize3_(vec3 v1);
/**
 *	\fn double distance3(vec3 v1, vec3 v2)
 *	\brief Fonction qui permet de calculer la distance entre deux vecteurs
 *	\param v1 Vecteur numéro 1
 *	\param v2 Vecteur numéro 2
 *	\return Retourne un double qui représente la distance entre les deux vecteurs
 */
 __device__ double distance3(vec3 v1, vec3 v2);
 double distance3_(vec3 v1, vec3 v2);
/**
 *	\fn double dot3(vec3 v1, vec3 v2)
 *	\brief Fonction qui permet de calculer le produit scalaire de deux vecteurs
 *	\param v1 Vecteur numéro 1
 *	\param v2 Vecteur numéro 2
 *	\return Retourne un double qui représente le produit scalaire de deux vecteurs
 */
 __device__ double dot3(vec3 v1, vec3 v2);
double dot3_(vec3 v1, vec3 v2);
/**
 *	\fn vec3 cross3(vec3 v1, vec3 v2)
 *	\brief Fonction qui permet de calculer le produit scalaire de deux vecteurs
 *	\param v1 Vecteur numéro 1
 *	\param v2 Vecteur numéro 2
 *	\return Retourne un double qui représente le produit scalaire de deux vecteurs
 */
 __device__ vec3 cross3(vec3 v1, vec3 v2);

/**
 *	\fn vec3 reflect3(vec3 v, vec3 vnormal)
 *	\brief Fonction qui permet de calculer le produit scalaire de deux vecteurs
 *	\param v Vecteur numéro 1
 *	\param vnormal Vecteur numéro 2
 *	\return Retourne un double qui représente le produit scalaire de deux vecteurs
 */
 __device__ vec3 reflect3(vec3 v, vec3 vnormal);
 vec3 reflect3_(vec3 v, vec3 vnormal);
/**
 *	\fn vec3 set3(double x, double y, double z )
 *	\brief Fonction qui permet d'initialiser un vecteur
 *	\param x Coordonnée du vecteur en x
 *	\param y Coordonnée du vecteur en y
 *	\param z Coordonnée du vecteur en z
 *	\return Retourne le vecteur composé des coordonnées x y et z
 */
 __device__ vec3 set3(double x, double y, double z);

/**
 *	\fn vec3 copy3(vec3 v)
 *	\brief Fonction qui permet de copier un vecteur
 *	\param v Vecteur a copier
 *	\return Retourne le vecteur copié
 */
 __device__ vec3 copy3(vec3 v);

#endif // !LIB_H