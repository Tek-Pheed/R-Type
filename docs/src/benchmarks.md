# Étude comparative

## Langages de programmation

### 1. Performance

- **C++** : Excellentes performances, en particulier pour les applications critiques telles que les jeux vidéo, grâce à un contrôle précis de la mémoire et des ressources. C'est le langage de choix pour les systèmes en temps réel nécessitant une faible latence et des calculs rapides.

- **Rust** : Offre des performances proches de celles du C++ avec des garanties de sécurité de la mémoire, évitant les erreurs courantes telles que les déréférences de pointeurs nuls ou les débordements de mémoire tampon.

- **Go** : Suffisant pour les serveurs de réseau, mais peut s'avérer difficile pour les graphiques en temps réel par rapport à C++ ou Rust.

- **Python** : Ne convient pas pour les parties critiques en termes de performances, comme le rendu en temps réel, mais est utile pour le prototypage ou l'écriture de scripts en raison de sa simplicité et de sa rapidité de développement.

### 2. Facilité de développement

- **C++** : Complexe en raison de la gestion manuelle de la mémoire et de la conception orientée objet avancée. Il exige une grande rigueur mais offre une immense flexibilité.

- **Rust** : Plus simple que le C++ pour la gestion de la mémoire, grâce à son système d'emprunt et de propriété.

- Go** : Moins complexe que C++ ou Rust mais offre moins de fonctionnalités pour la gestion fine des ressources.

- **Python** : Le plus facile à utiliser mais ne convient pas pour les parties critiques du jeu où les performances sont primordiales.

### 3. Concurrence et multithreading

- **C++** : Fournit une gestion fine des threads mais avec une complexité importante pour éviter les conditions de course et les blocages.

- **Rust** : Comprend un modèle de simultanéité sûr par défaut, évitant les erreurs d'accès aux threads et à la mémoire.

- **Go** : Excellent pour les applications réseau grâce à son modèle de goroutine léger, mais moins performant pour les tâches critiques telles que le rendu graphique.

- **Python** : Bien qu'il prenne en charge le multithreading, son verrouillage global de l'interpréteur (GIL) limite les performances pour les tâches liées au processeur. Cependant, il reste efficace pour les tâches liées aux entrées-sorties.

### 4. Support et écosystème

- **C++** : Possède un vaste écosystème de bibliothèques et d'outils, en particulier pour le développement de jeux vidéo (par exemple, SFML, SDL) et les moteurs de jeu.

- **Rust** : Son écosystème se développe, avec un nombre croissant de bibliothèques, mais il reste plus petit que celui de C++ pour le développement de jeux.

- **Go** : Principalement utilisé pour les réseaux et les applications dorsales, avec un écosystème solide dans ce domaine mais une utilisation limitée dans le développement de jeux.

- Python** : Il dispose d'un énorme écosystème pour les scripts, le prototypage et les outils d'automatisation, mais son utilisation est très limitée pour le développement de jeux en temps réel.

### Conclusion : Pourquoi C++ est le meilleur choix pour notre projet R-Type ?

Après avoir comparé plusieurs langages de programmation, le C++ apparaît comme le langage le plus approprié pour développer notre projet :

### Performance optimale
Le C++ est conçu pour les applications nécessitant une gestion fine des ressources et des performances, essentielles pour un jeu en réseau en temps réel. Il permet la gestion de la mémoire et l'optimisation du code pour répondre aux besoins de faible latence et d'exécution rapide, en particulier pour le rendu graphique et le multithreading.

### Contrôle total
Le C++ permet de contrôler tous les aspects du système, y compris la gestion des threads, l'accès direct à la mémoire et l'optimisation du code. Pour un projet comme R-Type, qui nécessite un serveur multithread, des communications réseau UDP et un moteur de jeu personnalisé, ce niveau de contrôle est crucial.

### Un écosystème riche pour les jeux vidéo
Le C++ est l'un des meilleurs langages pour créer des moteurs de jeu, avec une vaste bibliothèque d'outils (par exemple, SFML, SDL) et d'utilitaires (par exemple, CMake) qui facilitent le développement des jeux. Les principaux moteurs de jeu comme Unreal Engine sont basés sur le C++, ce qui renforce son aptitude à la conception de jeux.

### Portabilité et compatibilité multiplateforme
Le C++ garantit des performances élevées sur différents systèmes d'exploitation (Linux, Windows), ce qui constitue une contrainte pour le projet. L'utilisation d'outils tels que CMake et Conan pour la gestion des dépendances s'aligne davantage sur les capacités du C++.

### Exigences du projet Conformité
Le projet exige la création d'un moteur de jeu personnalisé sans utiliser de moteurs préexistants comme Unity. Le C++ est le langage idéal pour construire un moteur de jeu tout en garantissant performance et flexibilité.

### Verdict final
Le langage C++ est le choix idéal pour développer un jeu en réseau en temps réel avec des exigences élevées en matière de performances et de gestion fine des ressources. Il offre la flexibilité et la puissance nécessaires pour mener à bien un projet comme le nôtre.

## Comparaison des bibliothèques graphiques

### 1. SFML (Bibliothèque multimédia simple et rapide)

**Langage supporté :** C++

#### Avantages :
- Simple à utiliser et à intégrer, particulièrement bien adapté à un projet comme R-Type.
- Fournit des modules pour le rendu, la gestion des entrées, l'audio et le réseau, ce qui en fait une solution complète.
- Bien documenté et actif, avec une grande communauté.
- Multiplateforme (Windows, Linux, macOS).
- Un bon équilibre pour les projets nécessitant des éléments graphiques, audio et de mise en réseau.
- Compatible avec les solutions multithreading, nécessaires pour le serveur multijoueur du projet R-Type.

#### Inconvénients :
- Limité pour les jeux 3D ou les projets avec des besoins graphiques avancés.
- Moins performant que les bibliothèques spécialisées pour les tâches de rendu avancées (par exemple, OpenGL, SDL).

### 2. SDL (Simple DirectMedia Layer)

**Langage supporté :** C (avec des bindings pour C++ et d'autres langages)

#### Avantages :
- Très flexible et puissant, en particulier pour les projets 2D.
- Prend en charge de nombreuses plateformes (Windows, Linux, macOS, Android, iOS).
- Excellent pour le rendu de bas niveau.
- Utilisé dans de nombreux jeux (par exemple, *Divinity : Original Sin* et *Dead Cells*).
- Excellent pour la gestion des entrées et le rendu graphique.

#### Inconvénients :
- Ne fournit pas de modules intégrés pour l'audio et le réseau comme le fait la SFML, ce qui nécessite des bibliothèques supplémentaires.

### 3. Raylib

**Langage supporté :** C (avec des bindings pour plusieurs langues)

#### Avantages :
- Conçu pour les projets d'apprentissage ou le prototypage rapide.
- Supporte OpenGL pour un rendu graphique avancé.
- Performant pour les jeux en 2D avec des besoins simples.

#### Inconvénients :
- Moins complet que SFML en termes de fonctionnalités intégrées (par exemple, audio et réseau).
- Ne convient pas aux projets ayant des besoins complexes en matière de réseau, tels que les jeux multijoueurs.

### 4. OpenGL (avec GLFW pour la gestion des fenêtres)

**Langage supporté :** C/C++

#### Avantages :
- Haute performance pour les projets graphiques avancés, en particulier le rendu 3D.
- Contrôle total sur le pipeline graphique, idéal pour les effets personnalisés.

#### Inconvénients :
- Très complexe à utiliser.
- Nécessite des bibliothèques distinctes pour la gestion des entrées et l'audio (par exemple, GLFW).
- Excessif pour les jeux en 2D avec des besoins simples de mise en réseau.

### 5. Vulkan

**Langage supporté :** C/C++

#### Avantages :
- Excellente gestion des ressources et des performances, notamment pour les plateformes multicœurs.
- Multiplateforme (Windows, Linux, Android).
- Idéal pour les projets graphiques de haute performance (p. ex. moteurs de jeux 3D, simulations complexes).

#### Inconvénients :
- Ne convient pas aux projets simples ou aux prototypes en raison de sa complexité.

### 6. DirectX

**Langage supporté :** C++

#### Avantages :
- Haute performance sur Windows, avec un excellent support pour les jeux AAA.
- DirectX 12 offre des performances de type Vulkan et une gestion fine des ressources matérielles.

#### Inconvénients :
- Pas multiplateforme, limité à Windows et à la Xbox.
- Complexe, nécessitant un effort de développement important.

### 7. Allegro

**Langage supporté :** C/C++

#### Caractéristiques principales :
Allegro est une bibliothèque simple et multiplateforme conçue pour le développement de jeux en 2D. Elle inclut des fonctionnalités pour gérer le son, les entrées et les graphiques.

#### Avantages :
- Facile à utiliser, particulièrement adapté aux projets 2D et aux petites applications graphiques.
- Gère les entrées (clavier, souris, contrôleurs), le son et les graphiques.

#### Inconvénients :
- Moins de fonctionnalités avancées que SFML ou SDL.
- Moins utilisé que d'autres bibliothèques comme SFML ou SDL, ce qui réduit les ressources de la communauté.

## Conclusion

Pour le projet R-Type, nous avons choisi la bibliothèque SFML car c'est le choix idéal, offrant simplicité, polyvalence et rapidité de développement. Elle répond aux exigences du projet en matière de rendu 2D, de mise en réseau, d'audio et de gestion des entrées.

### Pourquoi SFML est un bon choix :

1. **Facilité d'utilisation** : Son API intuitive permet un apprentissage rapide, nous permettant de nous concentrer sur le développement du jeu sans courbe d'apprentissage complexe.

2. **Multimédia intégré** : La SFML gère le rendu 2D, le son, les entrées (clavier, souris, contrôleurs) et même le réseau, ce qui la rend idéale pour le projet R-Type sans nécessiter de multiples bibliothèques.

3. **Performance suffisante** : Bien que moins optimisée que les bibliothèques de bas niveau comme OpenGL ou Vulkan, la SFML est plus qu'adéquate pour un jeu multijoueur en 2D.

4. **Compatibilité multiplateforme** : La SFML fonctionne de manière transparente sous Windows, Linux et macOS, ce qui permet un développement multiplateforme sans effort supplémentaire.

5. **Communauté et documentation** : Une documentation complète et une communauté active facilitent la résolution des problèmes.

En résumé, SFML nous permet de créer rapidement un prototype fonctionnel et un jeu complet par la suite. C'est un excellent choix pour un projet de jeu vidéo en 2D comme R-Type, où l'accent est mis sur le gameplay et la structure du réseau plutôt que sur des effets graphiques complexes.

## **Étude Comparative : Accessibilité et Sécurité des Technologies**

### **Accessibilité**

Dans cette analyse comparative, une attention particulière a été portée sur les fonctionnalités d'accessibilité de chaque technologie et leur capacité à répondre aux besoins des personnes en situation de handicap (PSH).

#### **Analyse des Langages**

- **C++** : Grâce à son écosystème étendu de bibliothèques, C++ offre une excellente flexibilité pour implémenter des fonctionnalités d'accessibilité. Des bibliothèques comme **SFML (Simple and Fast Multimedia Library)** permettent une approche conviviale pour créer des applications accessibles. La performance et la personnalisation de C++ en font un choix idéal pour des solutions nécessitant des fonctionnalités d'accessibilité sur mesure.
- **Rust** : Bien que Rust soit sûr et efficace, il dépend fortement de bibliothèques externes pour implémenter des fonctionnalités d'accessibilité, ce qui le rend moins accessible aux débutants dans ce domaine.
- **Go** : Sa simplicité est un avantage, mais Go manque de support natif ou d'un écosystème de bibliothèques robuste spécifiquement conçu pour l'accessibilité.
- **Python** : Python propose des bibliothèques de haut niveau comme **PyGame** ou **tkinter** qui simplifient le développement d'applications accessibles. Cependant, ses limitations en termes de performance peuvent freiner son utilisation pour des projets nécessitant beaucoup de ressources.

#### **Accessibilité dans les Bibliothèques Graphiques**

- **SFML (C++)** :  
  SFML se distingue comme une bibliothèque conviviale et très performante, facilitant la création d'applications accessibles. Son API claire permet aux développeurs d'intégrer des fonctionnalités telles que la navigation au clavier, des tailles de police personnalisables et une compatibilité avec les technologies d'assistance comme les lecteurs d'écran. La légèreté et la flexibilité de SFML en font un choix adapté pour le développement d'applications accessibles.

- **SDL (C++)** :  
  SDL offre un contrôle plus bas niveau que SFML, mais nécessite plus d'efforts pour intégrer des fonctionnalités d'accessibilité.

- **Raylib** :  
  Bien que facile à utiliser, Raylib manque de support natif pour l'accessibilité et n'est pas aussi flexible que SFML pour implémenter des solutions personnalisées.

- **OpenGL & Vulkan** :  
  Ces deux API graphiques sont puissantes, mais leur nature bas niveau rend la mise en œuvre de l'accessibilité plus complexe. Elles sont mieux adaptées aux projets où la performance est priorisée sur l'accessibilité.

#### **Pourquoi C++ et SFML ?**

C++ combiné avec SFML offre un excellent équilibre entre performance, flexibilité et accessibilité. La conception conviviale de SFML simplifie l'intégration des fonctionnalités d'accessibilité, tandis que l'écosystème étendu de C++ garantit que les outils nécessaires pour des fonctionnalités avancées sont disponibles. Cela en fait un choix supérieur pour les projets visant à répondre aux besoins des personnes en situation de handicap.

---

### **Aspects Légaux et Réglementaires**

Pour répondre aux besoins des PSH, il est essentiel de se conformer aux normes et réglementations en vigueur. Par exemple :

- **WCAG (Web Content Accessibility Guidelines)** : Ces directives internationales définissent des standards pour rendre les contenus accessibles, notamment pour les personnes handicapées. Bien que ces normes s'appliquent principalement au web, leurs principes peuvent être adaptés aux applications logicielles. Nous avons utilise des outils comme ([webaim.org](https://webaim.org/resources/contrastchecker/)) afin de respecter ces regles.
- **RGPD (Règlement Général sur la Protection des Données)** : La sécurité des données est un aspect crucial, surtout pour les applications qui traitent des informations personnelles de PSH. Le RGPD impose des obligations strictes en matière de protection des données et de respect de la vie privée.

L'utilisation de **C++ et SFML** permet de respecter ces normes, grâce à leur flexibilité et à leur capacité à intégrer des fonctionnalités de sécurité et d'accessibilité conformes aux réglementations.

---

### **Étude des Vulnérabilités de Sécurité Récentes dans les Technologies Benchmarkées**

#### **Langages de Programmation**

- **C++** :  
  C++ est un langage mature avec une longue histoire de développement. Bien que des vulnérabilités aient été identifiées par le passé, aucune faille critique récente spécifique à C++ n'a été rapportée. Cependant, la gestion manuelle de la mémoire en C++ peut introduire des vulnérabilités si elle n'est pas correctement maîtrisée. Suivre les bonnes pratiques de programmation est essentiel pour minimiser les risques.

- **Rust** :  
  En avril 2024, une faille de sécurité critique nommée **"BatBadBut"** a été découverte dans la bibliothèque standard de Rust, affectant toutes les versions antérieures à 1.77.2 sur Windows. Identifiée comme **CVE-2024-24576**, cette vulnérabilité permettait à un attaquant d'exécuter des commandes shell arbitraires en contournant le mécanisme d'échappement lors de l'invocation de fichiers batch via l'API Command. L'équipe de sécurité de Rust a rapidement publié la version 1.77.2 pour corriger cette faille. ([lejournalduhack.com](https://lejournalduhack.com/une-faille-de-securite-critique-baptisee-batbadbut-a-ete-decouverte-dans-la-bibliotheque-standard-rust/))

- **Go** :  
  La vulnérabilité **"BatBadBut"** a également touché Go, permettant l'exécution de commandes shell arbitraires via des fichiers batch sur Windows. Les développeurs de Go ont publié des correctifs pour résoudre ce problème. ([lejournalduhack.com](https://lejournalduhack.com/une-faille-de-securite-critique-baptisee-batbadbut-a-ete-decouverte-dans-la-bibliotheque-standard-rust/))

- **Python** :  
  Python a également été affecté par la vulnérabilité **"BatBadBut"**, exposant les systèmes à des risques similaires d'exécution de commandes non autorisées. Des mises à jour ont été déployées pour corriger cette faille. ([lejournalduhack.com](https://lejournalduhack.com/une-faille-de-securite-critique-baptisee-batbadbut-a-ete-decouverte-dans-la-bibliotheque-standard-rust/))

#### **Bibliothèques Graphiques**

- **SFML (Simple and Fast Multimedia Library)** :  
  Aucune faille de sécurité critique récente n'a été rapportée pour SFML. Cependant, comme pour toute bibliothèque, il est recommandé de maintenir SFML à jour et de suivre les bonnes pratiques de programmation pour garantir la sécurité des applications.

- **SDL (Simple DirectMedia Layer)** :  
  SDL est une bibliothèque mature et largement adoptée. Aucune vulnérabilité critique récente n'a été rapportée. Néanmoins, il est essentiel de rester vigilant et de mettre régulièrement à jour la bibliothèque pour bénéficier des derniers correctifs de sécurité.

- **OpenGL et Vulkan** :  
  Ces API graphiques de bas niveau sont largement utilisées. Bien que des vulnérabilités puissent survenir dans des implémentations spécifiques ou des pilotes graphiques, aucune faille majeure inhérente n'a été récemment rapportée pour ces API. Il est crucial de maintenir les pilotes et les implémentations à jour pour assurer la sécurité.

---

### **Participation à la Communauté Professionnelle**

Pour rester à jour sur les évolutions technologiques, légales et réglementaires, il est important de participer activement à des événements et des rencontres professionnelles. Par exemple :

- **Conférences sur l'accessibilité** : Participer à des événements comme **Accessibility Days** ou **Global Accessibility Awareness Day (GAAD)** permet de se tenir informé des dernières tendances et bonnes pratiques en matière d'accessibilité.
- **Communautés de sécurité informatique** : Rejoindre des groupes comme **OWASP (Open Web Application Security Project)** ou participer à des conférences comme **Black Hat** aide à rester informé des dernières vulnérabilités et des méthodes pour les contrer.

---

### **Conclusion**

Parmi les technologies évaluées, **C++ et SFML** se distinguent par leur robustesse en termes de sécurité et d'accessibilité, avec peu ou pas de vulnérabilités critiques récentes rapportées. En revanche, des langages comme Rust, Go et Python ont été touchés par la vulnérabilité **"BatBadBut"** en 2024, soulignant l'importance de maintenir les environnements de développement à jour. Le choix de **C++ et SFML** offre une base solide pour le développement d'applications sécurisées et accessibles, à condition de suivre les bonnes pratiques de programmation et de surveiller les vulnérabilités potentielles.