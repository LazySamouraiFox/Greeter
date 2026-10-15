# CI/CD pas a pas

Ce dossier contient les workflows GitHub Actions **etape par etape**.
Chaque fichier ajoute une seule nouveaute par rapport au precedent
(les lignes changees sont marquees `# NOUVEAU` ou `# MODIFIE`).

Ces fichiers **ne sont pas executes** ici : GitHub ne lit que le dossier
`.github/workflows/`. Pour une etape, copiez le fichier au bon endroit :

```bash
mkdir -p .github/workflows
cp cours/cicd/01-ci-minimal.yml .github/workflows/ci.yml
git add .github/workflows/ci.yml
git commit -m "ci: add minimal CI"
git push
```

Puis ouvrez l'onglet **Actions** du depot sur GitHub.

| Etape | Fichier | Copier vers | Nouveaute |
|---|---|---|---|
| 1 | `01-ci-minimal.yml` | `.github/workflows/ci.yml` | compiler, tester, lancer a chaque push |
| 2 | `02-ci-pull-request.yml` | `.github/workflows/ci.yml` | CI sur les Pull Requests |
| 3 | `03-ci-matrix.yml` | `.github/workflows/ci.yml` | Linux **et** Windows |
| 4 | `04-ci-docker.yml` | `.github/workflows/ci.yml` | construire l'image Docker |
| 5 | `05-cd-release.yml` | `.github/workflows/cd.yml` | Release sur un tag `v*` |
| 6 | `06-cd-docker.yml` | `.github/workflows/cd.yml` | publier l'image sur ghcr.io |
| 7 | `07-semantic-release.yml` | `.github/workflows/ci.yml` (+ supprimer `cd.yml`) | version calculee depuis les commits |
| 8 | `08-bonus-format.yml` | `.github/workflows/format.yml` | verifier le formatage |

Dans chaque fichier, les jobs sont ranges sous un bandeau **CI** (verifier chaque
changement) ou **CD** (livrer une version), et leur nom commence par `CI - ` ou `CD - ` :
on retrouve ce prefixe dans l'onglet **Actions** et sur les Pull Requests.

Les etapes 1 a 4 remplacent toutes le **meme** fichier `ci.yml`.
Les etapes 5-6 creent un **deuxieme** fichier `cd.yml` (on choisit la version a la main avec un tag).
A l'etape 7, la machine choisit la version : tout revient dans `ci.yml` et `cd.yml` disparait.

---

## Etape 1 : CI minimale

**But :** comprendre qu'une CI n'est qu'une machine qui tape vos commandes.

1. Copiez `01-ci-minimal.yml` vers `.github/workflows/ci.yml`, commit, push.
2. Dans **Actions**, ouvrez le run et depliez chaque etape : vous retrouvez
   la sortie de `cmake`, du compilateur, de `ctest` et du programme :
   ```
   Hello Alice!
   built on Linux with GNU 13.x
   ```

**A observer :** la coche verte a cote du commit sur la page d'accueil du depot.

## Etape 2 : Pull Requests et "casser la CI"

**But :** voir la CI bloquer une erreur avant qu'elle n'arrive sur `master`.

1. Copiez `02-ci-pull-request.yml` vers `.github/workflows/ci.yml`, commit, push.
2. Protegez `master` : *Settings > Branches > Add branch protection rule*
   (ou *Rulesets*), branche `master`, cochez
   **Require status checks to pass before merging** et choisissez `CI - Compiler et tester`
   (le check n'apparait dans la liste qu'apres un premier run du workflow).
3. Creez une branche et cassez le code :
   ```bash
   git switch -c casse-tout
   # dans src/greeter/message.cpp : remplacez "Hello " par "Helo "
   git commit -am "fix: typo"
   git push -u origin casse-tout
   ```
4. Ouvrez une Pull Request vers `master`.

**A observer :** la croix rouge, le message `FAILED: got "Helo World!", expected "Hello World!"`
dans les logs de `ctest`, et le bouton *Merge* bloque.
Corrigez, poussez a nouveau : la PR redevient verte.

## Etape 3 : plusieurs systemes

**But :** CMake rend le projet portable, la CI le prouve.

Copiez `03-ci-matrix.yml` vers `.github/workflows/ci.yml`.

**A observer :** deux jobs `CI - Compiler et tester (ubuntu-latest)` et
`CI - Compiler et tester (windows-latest)`. Comparez les logs : `make` d'un
cote, Visual Studio (MSBuild) de l'autre, pour le **meme** `CMakeLists.txt`.
Et surtout la sortie du programme, qui sait sur quoi il a ete compile :

| Job | Sortie de `greet` |
|---|---|
| `ubuntu-latest` | `built on Linux with GNU 13.x` |
| `windows-latest` | `built on Windows with MSVC 19.x` |

> Attention : les jobs s'appellent maintenant `CI - Compiler et tester (ubuntu-latest)`
> et `CI - Compiler et tester (windows-latest)`. Dans la protection de `master` (etape 2),
> remplacez `CI - Compiler et tester` par ces deux noms, sinon GitHub attend un job qui
> n'existe plus et les Pull Requests restent bloquees.

## Etape 4 : Docker dans la CI

**But :** verifier a chaque changement que l'image Docker fonctionne.

Copiez `04-ci-docker.yml` vers `.github/workflows/ci.yml`.

**A observer :** le job `CI - Image Docker` attend la fin de `CI - Compiler et tester`
(`needs`). Si un test echoue, l'image n'est pas construite.
Ajoutez aussi `CI - Image Docker` aux checks obligatoires de `master`.

**Question :** vous etes sous Windows, et pourtant `docker run --rm greeter`
affiche `built on Linux`. Pourquoi ?

## Etape 5 : premiere Release (CD)

**But :** livrer automatiquement une version a partir d'un tag.

1. Copiez `05-cd-release.yml` vers `.github/workflows/cd.yml`, commit, push.
2. Creez et poussez un tag (format `vMAJEUR.MINEUR.CORRECTIF`) :
   ```bash
   git tag v1.0.0
   git push origin v1.0.0
   ```

**A observer :** un run `CD` dans **Actions**, puis l'onglet **Releases**
avec l'archive `Greeter-1.0.0-Linux.tar.gz` (creee par CPack).
La version de l'archive vient du tag : le workflow la passe a CMake avec
`-DCPACK_PACKAGE_VERSION=...`.

## Etape 6 : publier l'image Docker

**But :** n'importe qui peut lancer le programme sans le compiler.

1. Copiez `06-cd-docker.yml` vers `.github/workflows/cd.yml`, commit, push.
2. Poussez un nouveau tag (`v1.1.0`).
3. Sur n'importe quelle machine avec Docker :
   ```bash
   docker run --rm ghcr.io/<votre-compte>/<votre-depot>:v1.1.0
   ```

   Le nom de l'image est celui du depot **en minuscules** (exigence de ghcr.io).
   Pour passer un prenom : `docker run --rm ghcr.io/<votre-compte>/<votre-depot>:v1.1.0 greet Alice`
   (ce qui suit le nom de l'image remplace le `CMD` du Dockerfile).

**A observer :** l'image dans l'onglet **Packages** du depot.
Une image publiee par la CI est **privee** par defaut : `docker run` repond `denied`
ou `unauthorized`. Deux solutions :

- rendre le package public (*Package settings > Change visibility > Public*) ;
- ou se connecter avant avec un **token GitHub** (il remplace le mot de passe) :
  ```bash
  gh auth refresh -s read:packages
  gh auth token | docker login ghcr.io -u <votre-compte> --password-stdin
  ```
  Sans la CLI `gh` : creer un token **classic** (les tokens *fine-grained* ne marchent pas
  avec ghcr.io) dans *Settings > Developer settings > Personal access tokens > Tokens (classic)*,
  cocher `read:packages`, puis `docker login ghcr.io -u <votre-compte>` et coller le token.

## Etape 7 : semantic-release, la version automatique

**But :** ne plus choisir la version a la main. La machine la calcule a partir
des messages de commit, cree le tag et la Release, puis publie l'archive et l'image.

1. Copiez `07-semantic-release.yml` vers `.github/workflows/ci.yml`.
2. Supprimez `.github/workflows/cd.yml` (le tag n'est plus pousse a la main).
3. Ajoutez `.releaserc.json` a la racine (il est deja dans ce depot).
4. Commit, push sur une branche, Pull Request, merge.

Le fichier contient maintenant les deux parties : les jobs **CI** (sur chaque PR)
et le job **CD** `CD - Publier une version`, qui ne tourne qu'apres un merge sur
`master`, si tous les jobs de CI sont verts. Sur une Pull Request, il apparait
comme *skipped* : c'est normal. semantic-release lit alors les commits depuis le dernier tag :

| Message de commit | Exemple | Nouvelle version (depuis 1.2.3) |
|---|---|---|
| `fix: ...` | `fix: missing space in message` | `1.2.4` (correctif) |
| `feat: ...` | `feat: add get_goodbye` | `1.3.0` (fonctionnalite) |
| `BREAKING CHANGE: ...` dans le corps du commit | `feat: rename get_message` + ligne `BREAKING CHANGE: get_message is now get_hello` | `2.0.0` (rupture) |
| `docs:`, `ci:`, `test:`, `refactor:`... | `docs: explain options` | pas de nouvelle version |

**A observer :** dans le graphe du run, les jobs `CI - ...` puis `CD - Publier une version`, puis dans **Releases** une version avec
ses notes generees a partir des commits, l'archive `Greeter-X.Y.Z-Linux.tar.gz`,
et dans **Packages** l'image `X.Y.Z` et `latest`.

**Exercice :** merger une PR `docs: ...` (pas de version), puis une PR
`feat: ...` (nouvelle version mineure). Comparez les deux runs.

**Pieges :**

- avec le bouton *Merge pull request*, le commit sur `master` s'appelle
  `Merge pull request #12...`, qui n'est pas un Conventional Commit : seuls les
  commits de la branche comptent. Plus simple : choisir **Squash and merge** et
  donner a la PR un titre `feat: ...` ou `fix: ...` ;
- aucun token personnel n'est necessaire : le `GITHUB_TOKEN` suffit, car tout
  se passe dans le **meme** workflow (un tag cree par le `GITHUB_TOKEN` ne
  declencherait pas un autre workflow) et semantic-release ne fait aucun commit
  sur `master` (pas de plugin `@semantic-release/git`).

## Etape 8 (bonus) : formatage

**But :** automatiser les regles de style du projet (`.clang-format`).

Copiez `08-bonus-format.yml` vers `.github/workflows/format.yml`.

Le code du projet est deja formate : le job passe au vert.

**Exercice :** sur une branche, deformez volontairement un fichier
(par exemple `#include<greeter/message.h>` sans espace, ou une accolade sur la
meme ligne que `int main()`), ouvrez une Pull Request : le job echoue et
indique le fichier et la ligne. Corrigez en local :

```bash
clang-format -i apps/*.cpp src/greeter/*.cpp include/greeter/*.h tests/*.cpp tests/*.h
```

puis commit, push : le job repasse au vert.
