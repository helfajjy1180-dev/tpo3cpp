#include <algorithm>
#include <iostream>
#include <string>
#include <utility>

class CompteBancaire {
private:
    std::string numero;
    double solde;
    std::string* historique;
    std::size_t capacite;
    std::size_t nbOps;

    void agrandirHistorique() {
        const std::size_t nouvelleCap = (capacite == 0) ? 1 : capacite * 2;
        std::string* nouveau = new std::string[nouvelleCap];

        for (std::size_t i = 0; i < nbOps; ++i) {
            nouveau[i] = historique[i];
        }

        delete[] historique;
        historique = nouveau;
        capacite = nouvelleCap;
    }

    void ajouterOperation(const std::string& operation) {
        if (nbOps == capacite) {
            agrandirHistorique();
        }
        historique[nbOps++] = operation;
    }

public:
    // Constructeur par défaut
    CompteBancaire()
        : numero("NON_DEFINI"), solde(0.0),
          historique(new std::string[10]), capacite(10), nbOps(0) {
        ajouterOperation("Creation du compte avec solde 0");
    }

    // Constructeur parametre
    CompteBancaire(const std::string& num, double s)
        : numero(num), solde(s < 0 ? 0.0 : s),
          historique(new std::string[10]), capacite(10), nbOps(0) {
        if (s < 0) {
            std::cerr << "Solde initial negatif : solde fixe a 0." << std::endl;
        }
        ajouterOperation("Creation du compte avec solde " + std::to_string(solde));
    }

    // Constructeur de copie : copie profonde
    CompteBancaire(const CompteBancaire& autre)
        : numero(autre.numero), solde(autre.solde),
          historique(new std::string[autre.capacite]),
          capacite(autre.capacite), nbOps(autre.nbOps) {
        for (std::size_t i = 0; i < nbOps; ++i) {
            historique[i] = autre.historique[i];
        }
    }

    // Constructeur de deplacement
    CompteBancaire(CompteBancaire&& autre) noexcept
        : numero(std::move(autre.numero)), solde(autre.solde),
          historique(autre.historique), capacite(autre.capacite),
          nbOps(autre.nbOps) {
        autre.historique = nullptr;
        autre.capacite = 0;
        autre.nbOps = 0;
        autre.solde = 0.0;
        autre.numero = "DEPLACE";
    }

    // Affectation par copie
    CompteBancaire& operator=(const CompteBancaire& autre) {
        if (this != &autre) {
            std::string* nouveau = new std::string[autre.capacite];
            for (std::size_t i = 0; i < autre.nbOps; ++i) {
                nouveau[i] = autre.historique[i];
            }

            delete[] historique;
            numero = autre.numero;
            solde = autre.solde;
            historique = nouveau;
            capacite = autre.capacite;
            nbOps = autre.nbOps;
        }
        return *this;
    }

    // Affectation par deplacement
    CompteBancaire& operator=(CompteBancaire&& autre) noexcept {
        if (this != &autre) {
            delete[] historique;
            numero = std::move(autre.numero);
            solde = autre.solde;
            historique = autre.historique;
            capacite = autre.capacite;
            nbOps = autre.nbOps;

            autre.historique = nullptr;
            autre.capacite = 0;
            autre.nbOps = 0;
            autre.solde = 0.0;
            autre.numero = "DEPLACE";
        }
        return *this;
    }

    // Destructeur
    ~CompteBancaire() {
        delete[] historique;
    }

    void deposer(double montant) {
        if (montant <= 0) {
            std::cerr << "Montant de depot invalide." << std::endl;
            return;
        }
        solde += montant;
        ajouterOperation("Depot de " + std::to_string(montant));
    }

    void retirer(double montant) {
        if (montant <= 0 || montant > solde) {
            std::cerr << "Retrait impossible." << std::endl;
            return;
        }
        solde -= montant;
        ajouterOperation("Retrait de " + std::to_string(montant));
    }

    void afficher() const {
        std::cout << "\\nCompte : " << numero << "\\nSolde : " << solde << std::endl;
        std::cout << "Historique :" << std::endl;
        for (std::size_t i = 0; i < nbOps; ++i) {
            std::cout << "- " << historique[i] << std::endl;
        }
    }
};

int main() {
    CompteBancaire c1("FR123456", 500.0);
    c1.deposer(200.0);
    c1.retirer(150.0);
    c1.afficher();

    std::cout << "\\n--- Copie de c1 ---" << std::endl;
    CompteBancaire c2 = c1;
    c2.deposer(50.0);
    c2.afficher();

    std::cout << "\\n--- Deplacement de c1 vers c3 ---" << std::endl;
    CompteBancaire c3 = std::move(c1);
    c3.afficher();

    return 0;
}
