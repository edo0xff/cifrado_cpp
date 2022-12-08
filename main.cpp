#include <ctime>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

/**
 * @brief Esta funcion encripta o desencripta una cadena de datos.
 * 
 * El algoritmo es una operacion sencilla (suma para encriptar
 * y resta para desencriptar) que se puede escribir de la siguiente
 * forma:
 * 
 * Cn = (On + Km) % 256
 * On = (Cn - Km) % 256
 * 
 * Siendo C un Byte cifrado, O el Byte original y K el Byte de la
 * clave de cifrado.
 * 
 * @param data Cadena de datos para cifrar o decifrar
 * @param key  Cadena de la clave de cifrado
 * @param revert Si true entonces decifra, si false entonces cifra
 * @return const string 
 */
const string crypt(string data, string key, bool revert=false) {
    string result;
    size_t j = 0;

    // El algoritmo aplica la funcion (ya sea suma o resta)
    // entre cada byte de la cadena y la clave
    for (size_t i = 0; i < data.size(); i++)
    {
        // Ya que la clave es menor en longitud que los datos
        // esta se 'repite' hasta cubrir la longitud de los datos
        if (j >= key.size()) j = 0;
        
        int c = (int)data.at(i);
        int k = (int)key.at(j);
        // La operacion modular (256) se aplica para no 
        // sobrepasar el tamanio maximo de un byte
        int o = revert ? (c - k) % 256 : (c + k) % 256;

        result += (char)o;
        j++;
    }

    return result;
}

/**
 * @brief Esta funcion procesa el archivo de entrada para cifrar/decifrar
 * 
 * Dependiendo del parametro @revert, si true, decifra el archivo
 * si false entonces lo cifra.
 * 
 * @param ipath Path del archivo para cifrar
 * @param opath Path del archivo de salida de los datos cifrados/decifrados
 * @param key Clave de cifrado/decifrado
 * @param revert Si true, entonces decifra, si false entonce cifra.
 */
void process(string ipath, string opath, string key, bool revert=false) {
    ifstream ifile(ipath, ifstream::binary | ifstream::ate);
    
    auto file_size = ifile.tellg();
    ifile.seekg(0);

    if (ifile) {
        ofstream ofile(opath, ofstream::binary | ofstream::ate);

        time_t start_time = time(0);
        
        time_t elapsed_time = 0;
        time_t current_elapsed_time = 0;
        
        uint32_t progress = 0;
        uint32_t current_progress = 0;

        // Para optimizar el uso de memoria con archivos grandes
        // el archivo se lee y cifra/decifra en bloques de 
        // 4092 Bytes
        uint32_t buffer_size = 4092;
        uint32_t processed_bytes_count = 0;

        for(;;) {
            // Los Bytes del ultimo bloque puede que sean menos
            // que los Bytes del tamnio del buffer, hay que calcular
            // cuantos Bytes realmente tiene el ultimo bloque 
            // y ajustar el tamanio del buffer
            if ((processed_bytes_count + buffer_size) > file_size)
                buffer_size = buffer_size - ((processed_bytes_count + buffer_size) - file_size);
            
            string buffer(buffer_size, '\0');

            ifile.read(&buffer[0], buffer_size);
            std::streamsize readed_bytes_count = ifile.gcount();

            buffer = crypt(buffer, key, revert);
            ofile << buffer;

            processed_bytes_count += readed_bytes_count;
            
            elapsed_time = time(0) - start_time;
            progress = (int)((100.f/(float)file_size) * (float)processed_bytes_count);

            // Se imprime el progreso solo si el tiempo de ejecucion
            // o el progreso en si es distinto a la ultima vez que se 
            // imprimio, esto para optimizar el uso de memoria de video
            if ((progress > current_progress) || (elapsed_time > current_elapsed_time)) {

                char char_just_for_fun = buffer.at(0);

                if ((int)char_just_for_fun < 16)
                    char_just_for_fun = '?';

                cout << "\r[" << char_just_for_fun << "] [";
                cout << string((progress/2), '/') << string(50 - (progress/2), ' ') << "] ";
                cout << progress << "% (" << elapsed_time << " secs)";

                current_progress = progress;
                current_elapsed_time = elapsed_time;
            }
            
            if (processed_bytes_count == file_size) break;
        }
        
        ofile.close();
        ifile.close();

        cout << endl << "[i] " << processed_bytes_count << " Bytes processed" << endl;
        cout << "[i] Saved (" << opath << ")";

        return;
    }

    cout << "[!] Error: can't open file: " << ipath;
}

int main(int argc, char** argv) {
    cout << "[i] C++ Simple crypto tool" << endl;
    cout << "[i] coded by <edo0xff>" << endl;

    if (argc < 4) {
        cout << "[!] Missing parameters: " << endl;
        cout << "    main.exe -c input_file output_crypted_file" << endl;
        cout << "    main.exe -d input_crypted_file output_original_file" << endl;
        return 0;
    }

    string clave;
    cout << "[i] type a crypt key: ";
    getline(cin, clave);

    string ipath = argv[2];
    string opath = argv[3];

    string option = argv[1];

    if (option == "-c") {
        cout << "[i] Encrypting (" << ipath << ")... " << endl;
        process(ipath, opath, clave, false);
    }

    else if (option == "-d") {
        cout << "[i] Decrypting (" << ipath << ")... " << endl;
        process(ipath, opath, clave, true);
    } 
    
    else {
        cout << "[!] Error, invalid option: " << option << endl;
    }

    return 0;
}