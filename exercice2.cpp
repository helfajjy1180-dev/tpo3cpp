#include <algorithm>
#include <cstddef>
#include <iostream>
#include <utility>

class Buffer {
private:
    char* data;
    std::size_t taille;

public:
    // Constructeur par defaut
    Buffer() : data(nullptr), taille(0) {}

    // Constructeur parametre
    explicit Buffer(std::size_t t)
        : data(t == 0 ? nullptr : new char[t]), taille(t) {
        if (data != nullptr) {
            std::fill_n(data, taille, '\0');
        }
        std::cout << "Buffer de " << taille << " octets cree." << std::endl;
    }

    // Constructeur de copie : copie profonde
    Buffer(const Buffer& autre)
        : data(autre.taille == 0 ? nullptr : new char[autre.taille]),
          taille(autre.taille) {
        if (taille > 0) {
            std::copy(autre.data, autre.data + taille, data);
        }
        std::cout << "Constructeur de copie appele." << std::endl;
    }

    // Constructeur de deplacement
    Buffer(Buffer&& autre) noexcept
        : data(autre.data), taille(autre.taille) {
        autre.data = nullptr;
        autre.taille = 0;
        std::cout << "Constructeur de deplacement appele." << std::endl;
    }

    // Affectation par copie
    Buffer& operator=(const Buffer& autre) {
        if (this != &autre) {
            char* nouveau = autre.taille == 0 ? nullptr : new char[autre.taille];
            if (autre.taille > 0) {
                std::copy(autre.data, autre.data + autre.taille, nouveau);
            }
            delete[] data;
            data = nouveau;
            taille = autre.taille;
        }
        return *this;
    }

    // Affectation par deplacement
    Buffer& operator=(Buffer&& autre) noexcept {
        if (this != &autre) {
            delete[] data;
            data = autre.data;
            taille = autre.taille;
            autre.data = nullptr;
            autre.taille = 0;
        }
        return *this;
    }

    // Destructeur
    ~Buffer() {
        delete[] data;
        std::cout << "Destructeur du buffer appele." << std::endl;
    }

    char* getData() const {
        return data;
    }

    std::size_t getTaille() const {
        return taille;
    }

    void remplir(char valeur) {
        if (data != nullptr) {
            std::fill_n(data, taille, valeur);
        }
    }

    void afficher() const {
        std::cout << "Taille du buffer : " << taille << " octets" << std::endl;
        if (taille > 0) {
            std::cout << "Premier caractere : " << data[0] << std::endl;
        }
    }
};

int main() {
    Buffer b1(128);
    b1.remplir('A');
    b1.afficher();

    std::cout << "\\n--- Copie de b1 ---" << std::endl;
    Buffer b2 = b1;
    b2.afficher();

    std::cout << "\\n--- Deplacement de b1 vers b3 ---" << std::endl;
    Buffer b3 = std::move(b1);
    b3.afficher();
    std::cout << "Taille de b1 apres deplacement : " << b1.getTaille() << std::endl;

    std::cout << "\\n--- Affectation par copie ---" << std::endl;
    b2 = b3;
    b2.afficher();

    std::cout << "\\n--- Affectation par deplacement ---" << std::endl;
    b3 = Buffer(64);
    b3.afficher();

    return 0;
}
