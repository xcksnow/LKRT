#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <Windows.h>

namespace fs = std::filesystem;

class FileSystemTree {
public:
    struct Archivo {
        std::string nombre;

        Archivo(const std::string& nombre) : nombre(nombre) {}
    };

    struct Carpeta {
        std::string nombre;
        std::vector<Archivo> archivos;
        std::vector<Carpeta> carpetas;

        Carpeta(const std::string& nombre) : nombre(nombre) {}
    };

    void llenarEstructuraDesdeDirectorio(const std::string& directorio, Carpeta& carpeta) {
        try {
            for (const auto& entrada : fs::directory_iterator(directorio)) {
                try {
                    if (entrada.is_directory()) {
                        if (!fs::is_symlink(entrada)) {
                            Carpeta subcarpeta(entrada.path().filename().string());
                            llenarEstructuraDesdeDirectorio(entrada.path().string(), subcarpeta);
                            carpeta.carpetas.push_back(subcarpeta);
                        }
                    }
                    else if (entrada.is_regular_file()) {
                        carpeta.archivos.push_back(Archivo(entrada.path().filename().string()));
                    }
                }
                catch (const std::filesystem::filesystem_error& e) {
                }
                catch (const std::system_error& e) {
                }
            }
        }
        catch (const fs::filesystem_error& e) {
        }
    }

    std::string generarStringEstructura(const Carpeta& carpeta, int nivel = 0) {
        std::string resultado;

        for (const auto& archivo : carpeta.archivos) {
            resultado += "-" + archivo.nombre;
        }

        for (const auto& subcarpeta : carpeta.carpetas) {
            resultado += ";";
            resultado += generarStringEstructura(subcarpeta, nivel + 1);
        }

        return resultado;
    }

    std::string getAllPaths(const std::string& directorioRaiz) {
        std::string resultado;

        try {
            for (const auto& entrada : fs::directory_iterator(directorioRaiz)) {
                if (entrada.is_regular_file()) {
                    resultado += "!" + entrada.path().filename().string();
                }
                else if (entrada.is_directory()) {
                    resultado += ";" + entrada.path().filename().string();
                }
            }
        }
        catch (const fs::filesystem_error& e) {
            std::cerr << "Error al acceder al directorio: " << e.what() << std::endl;
        }
        resultado = directorioRaiz + ";" + resultado;
        return resultado;
    }

    // Read File
    string readFile(const std::string& path) {
		string resultado;
		ifstream archivo(path);
		if (archivo.is_open()) {
			std::string linea;
			while (getline(archivo, linea)) {
				resultado += linea + "\n";
			}
			archivo.close();
		}
		return resultado;
	}
};