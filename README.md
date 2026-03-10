# TP4_Reproductibility

## Section 1.2.4 - Questions sur les hash et checksums

### Question 1 : Probabilité de collision avec SHA-256

La probabilité que deux chaînes différentes produisent le même hash SHA-256 est extrêmement faible, pour plusieurs raisons :

1. **Taille de l'espace des hash** : SHA-256 produit un hash de 256 bits, ce qui donne **$2^{256}$** possibilités (environ **$1.16 × 10^{77}$**). C'est un nombre astronomique, plus grand que le nombre d'atomes dans l'univers observable (estimé à $10^{80}$).

2. **Résistance aux collisions** : Pour trouver une collision avec 50% de chance, il faudrait environ **$2^{128}$** essais (paradoxe des anniversaires). Comme l'explique le PDF (page 6), même si tout le réseau Bitcoin (600 × $10^{18} hash/s) était consacré à cette tâche, il faudrait environ **18 milliards d'années** pour atteindre cette probabilité.

3. **Sécurité pratique** : Dans les faits, on considère que deux chaînes différentes ne produiront jamais le même hash SHA-256. C'est pourquoi cet algorithme est utilisé pour :
   - Vérifier l'intégrité des fichiers téléchargés
   - Assurer la reproductibilité des builds
   - Sécuriser la chaîne d'approvisionnement logicielle

### Question 2 : Recherche de collision avec l'algorithme simple

**Objectif :** Trouver deux chaînes différentes produisant le même checksum avec le script `checksum.py`.

#### Expérimentations réalisées

Plusieurs tests ont été effectués avec différentes chaînes :

| Chaîne | Checksum (12 caractères) |
|--------|---------------------------|
| `"a"`  | `989898989898` |
| `"b"`  | `999999999999` |
| `"aa"` | `487487487487` |
| `"ab"` | `491491491491` |
| `"ba"` | `488488488488` |
| `"abc"`| `138313831383` |
| `"acb"`| `137813781378` |
| `"cbc"`| `138513851385` |
| `"ccb"`| `138013801380` |

#### Résultat

Aucune collision évidente n'a été trouvée lors de ces tests. Cependant, cela ne signifie pas qu'il est impossible d'en trouver.

#### Pourquoi des collisions peuvent exister

Même si nous n'en avons pas trouvé, cet algorithme présente plusieurs faiblesses qui rendent les collisions possibles en théorie :

1. **Espace limité** : Le checksum fait 12 caractères → seulement \(10^{12}\) valeurs possibles, alors que le nombre de chaînes possibles est infini (principe des tiroirs de Dirichlet).

2. **Algorithme déterministe et simple** : Il repose sur une somme pondérée par la position. Différentes combinaisons de caractères peuvent produire la même somme.

3. **Perte d'information** : Le modulo et la répétition finale écrasent une partie des données.

### Question 3 : Utilisation de tar/gzip avant de hasher

**Réponse :**

Non, il n'est pas conseillé d'utiliser un outil comme `tar` ou `gzip` pour consolider un objet du système de fichiers avant de calculer son digest.

**Raisons :**

1. **Métadonnées incluses** : `tar` conserve les permissions, les timestamps, les UID/GID, etc. Ces métadonnées varient d'un système à l'autre et dans le temps, même si le contenu des fichiers est identique.
2. **Ordre non déterministe** : L'ordre dans lequel les fichiers sont ajoutés à l'archive peut varier selon le système de fichiers ou la commande utilisée.

3. **Compression non reproductible** : `gzip` ajoute des métadonnées (timestamp, nom du fichier original) dans l'en-tête, ce qui modifie le hash final.

**Conséquence :** Deux dossiers avec exactement les mêmes fichiers mais créés dans des conditions différentes produiront des hash différents.

**Alternative recommandée :** Utiliser `nix-hash` qui normalise la représentation du chemin avant de calculer le hash, garantissant ainsi la reproductibilité.


### Section 2.2.1 – Questions sur "Hello World"

#### Question 1

**Métadonnées du binaire compilé :**

Après compilation de `hello-world.c`, le fichier `hello-world` présente les métadonnées suivantes :

- **Taille** : 16008 octets
- **Permissions** : -rwxrwxr-x (exécutable par le propriétaire et le groupe)
- **Architecture** : ELF 64-bit LSB executable, x86-64
- **Type** : dynamically linked
- **Interpréteur** : /lib64/ld-linux-x86-64.so.2
- **BuildID** : 2af1ac8ccaed75fc3becb558eff20f6385d169c4
- **Non strippé** : les symboles de débogage sont présents

**Fichiers intermédiaires :**

La commande `gcc -o hello-world hello-world.c` ne génère **pas** de fichier intermédiaire visible. Le compilateur crée normalement un fichier objet (`.o`) temporaire mais le supprime automatiquement. Pour voir ce fichier intermédiaire, il faudrait compiler en deux étapes :

#### Question 2

- Avec `diff` on comparerait les sorties textuelles
- Avec `cmp` on comparerait les binaires
- Avec `sha256sum` on comparerait les hash

#### Question 3

**Observation :**  
En compilant le programme plusieurs fois, les dates/heures affichées sont différentes :

- Première compilation : `Mar 10 2026 17:07:51`
- Deuxième compilation : `Mar 10 2026 17:09:14`

**Explication :**  
Le programme utilise les macros préprocesseurs `__DATE__` et `__TIME__` qui sont évaluées **au moment de la compilation**. Chaque fois que le compilateur est invoqué, ces macros sont remplacées par la date et l'heure courantes du système.

Le binaire produit n'est donc pas identique d'une compilation à l'autre, car ces valeurs sont inscrites directement dans le code machine. C'est un exemple classique de **non-déterminisme à la compilation** : le même code source produit des exécutables différents selon le moment où on le compile.

#### Question 4

**Observation :**  
En exécutant le programme plusieurs fois sans le recompiler, la date/heure affichée est identique à chaque run :

#### Question 5

**Compatibilité du binaire partagé :**

- **Même architecture (ex: Linux x86_64) :**  
  Le binaire a de bonnes chances de fonctionner sur la machine d'un autre étudiant, **à condition** que les bibliothèques système (notamment `glibc`) soient compatibles. Des différences de version peuvent toutefois causer des erreurs.

- **Architecture différente (ex: Linux x86_64 → macOS ARM64) :**  
  Le binaire ne fonctionnera **pas**. Le code machine généré pour x86_64 n'est pas exécutable sur un processeur ARM, car les jeux d'instructions sont différents. Le système d'exploitation ne pourra pas l'interpréter.

**Conclusion :**  
Partager un binaire n'est fiable que si les machines cibles ont strictement la même architecture et des environnements système très proches. C'est pourquoi il est généralement préférable de partager le **code source** et laisser chaque machine le compiler.

#### Question 6

**Partager le binaire vs partager le code source**

Il est généralement **préférable de partager le code source** plutôt que le binaire, pour plusieurs raisons :

| Critère | Binaire | Code source |
|---------|---------|-------------|
| **Portabilité** | Limitée à une architecture spécifique (x86, ARM, etc.) | Peut être compilé sur n'importe quelle architecture |
| **Transparence** | On ne peut pas voir ce qu'il fait (boîte noire) | On peut inspecter, auditer, modifier |
| **Dépendances** | Dépend des bibliothèques système installées | On peut adapter la compilation à l'environnement |
| **Sécurité** | Risque si la provenance est inconnue | On peut vérifier le code avant de compiler |
| **Maintenance** | Difficile à mettre à jour | On peut recompiler avec les correctifs |

**Exceptions :**  
Partager un binaire peut se justifier pour :
- Distribuer une application à des utilisateurs non techniques
- Déployer sur des systèmes identiques (ex: clusters)
- Éviter la compilation sur des machines lentes ou sans outillage

Mais dans un contexte académique ou collaboratif, le code source reste la meilleure pratique.

#### Question 7 – Challenge : SOURCE_DATE_EPOCH

**Recherche :**

`SOURCE_DATE_EPOCH` est une variable d'environnement standardisée par le projet Reproducible Builds [citation:3]. Elle permet de fixer une date de compilation déterministe pour les outils de build qui la supportent.

Sa valeur est un timestamp UNIX (nombre de secondes depuis le 1er janvier 1970) [citation:3].

**Solution pour compiler `hello-world.c` de façon reproductible :**

Depuis GCC 7, le compilateur supporte nativement `SOURCE_DATE_EPOCH` [citation:8]. Quand cette variable est définie, GCC l'utilise pour initialiser les macros `__DATE__` et `__TIME__` au lieu de la date/heure système [citation:9].


### Section 2.3.1 – Questions sur le générateur pseudo-aléatoire

#### Question 1

**Observation :**  
En exécutant plusieurs fois le programme `random` (sans recompilation), on obtient toujours la même séquence de nombres :

#### Question 2

**Observation :**  
Comme montré dans la question précédente, exécuter plusieurs fois le programme `random` (sans recompilation) produit **strictement la même séquence** de nombres à chaque run.

**Reproductibilité du "hasard" :**  
Ce comportement montre que le hasard généré par `rand()` sans graine explicite est en réalité :

- **Déterministe** : la séquence est complètement prévisible
- **Reproductible** : on peut reproduire la même séquence à volonté
- **Dépendant de l'implémentation** : mais identique sur un même système

C'est pourquoi on parle de générateur **pseudo-aléatoire** : il produit des nombres qui semblent aléatoires, mais qui sont en réalité calculés par un algorithme déterministe.

### Section 2.4.1 – Questions sur le générateur avec seed basée sur le temps

#### Question 1

**Observation :**  
En compilant le programme à deux moments différents (à une minute d'intervalle), les séquences de nombres générées sont différentes :

| Compilation | Séquence |
|-------------|----------|
| 1ère (17:07) | 1895368372, 351943474, 725475974, 1541874884, 192499964 |
| 2ème (17:08) | 55563976, 74604774, 412024826, 1294703277, 1496077104 |

**Explication :**  
Le programme utilise `srand(time(NULL))` pour initialiser le générateur avec la valeur de l'horloge système **au moment de l'exécution**. Comme le temps a changé entre les deux exécutions, la graine est différente, donc la séquence est différente.

**Mais attention :** la question porte sur **plusieurs compilations**. Ici, on a :
1. Compilation → exécution immédiate → séquence A
2. Compilation (1 minute plus tard) → exécution immédiate → séquence B

Les séquences sont différentes car la graine change **entre les exécutions**. Si on exécutait le même binaire plusieurs fois sans le recompiler, on obtiendrait aussi des séquences différentes (c'est la question 2).

#### Question 2

**Observation :**  
En exécutant plusieurs fois le même binaire `random_seed` (sans recompilation), on obtient **la même séquence** à chaque run :

#### Question 3

**Comportement à l'exécution :**

Oui, cette version du programme se comporte différemment de la version sans seed (Section 2.3) pour plusieurs raisons :

| Version | Comportement |
|---------|--------------|
| Sans seed (`random.c`) | Produit la **même séquence** à chaque exécution (graine fixe par défaut) |
| Avec seed temporelle (`random_seed.c`) | Produit des séquences **différentes** si les exécutions sont espacées d'au moins 1 seconde |

**Explication :**
- `srand(time(NULL))` réinitialise le générateur avec la valeur courante de l'horloge
- Chaque seconde qui passe change la graine → nouvelle séquence
- Si les exécutions sont très rapides (< 1s), la graine est identique → même séquence (comportement trompeur)

**Conséquence pour la reproductibilité :**
- Version sans seed : **runtime reproductible** (toujours la même sortie)
- Version avec seed temporelle : **runtime non reproductible** (sauf cas particulier d'exécutions très rapprochées)

Ce comportement illustre l'importance de **contrôler explicitement les sources d'aléatoire** quand on veut des résultats reproductibles.

