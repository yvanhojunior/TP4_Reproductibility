# TP4_Reproductibility

## Section 1.2.4 - Questions sur les hash et checksums

### Question 1 : Probabilité de collision avec SHA-256

La probabilité que deux chaînes différentes produisent le même hash SHA-256 est extrêmement faible, pour plusieurs raisons :

1. **Taille de l'espace des hash** : SHA-256 produit un hash de 256 bits, ce qui donne **$$2^256$$** possibilités (environ **1.16 × 10^77**). C'est un nombre astronomique, plus grand que le nombre d'atomes dans l'univers observable (estimé à 10^80).

2. **Résistance aux collisions** : Pour trouver une collision avec 50% de chance, il faudrait environ **2^128** essais (paradoxe des anniversaires). Comme l'explique le PDF (page 6), même si tout le réseau Bitcoin (600 × 10^18 hash/s) était consacré à cette tâche, il faudrait environ **18 milliards d'années** pour atteindre cette probabilité.

3. **Sécurité pratique** : Dans les faits, on considère que deux chaînes différentes ne produiront jamais le même hash SHA-256. C'est pourquoi cet algorithme est utilisé pour :
   - Vérifier l'intégrité des fichiers téléchargés
   - Assurer la reproductibilité des builds
   - Sécuriser la chaîne d'approvisionnement logicielle
