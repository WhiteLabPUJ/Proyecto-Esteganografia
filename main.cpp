#include <Windows.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <cstring>
#include <stdio.h>
#include <stdio.h>
#include <string>
#include "conio.h"
#include "CImg.h"

using namespace std;
using namespace cimg_library;

#define FLAG          0x7e
#define ADDRESS       0xFF
#define INFO          0x02
#define CONTROL       0x03
#define PROTOCOL1     0x80
#define PROTOCOL2     0x49
#define TEST_PROTOCOL 0x8049


struct Bits
{
    unsigned b8:1, b7:1, b6:1, b5:1, b4:1, b3:1, b2:1, b1:1;
};
union cool_char
{
    Bits bits;
    unsigned char letra;
};

string add_flag(string& original,string& name);
bool check(CImg<unsigned char>* ptr,int& line);
void pereza();
void encode();
void decode();
int menu();

int menu()
{
    int decision;
    pereza();
    cout << "Bienvenido, por favor seleccione una de las siguientes opciones: "<<endl;
    cout << "   1) Ocultar archivos en una imagen" << endl;
    cout << "   2) Extraer archivos de una imagen" << endl;
    cout << "   3) Salir" << endl;
    cin>>decision;
    return(decision);
}

int main()
{
    system("cls");
    int dec;
    bool flag = true;
    dec = menu();
    while(flag)
    {
        if(dec == 1)
        {
            system("cls");
            encode();
        }
        else if(dec == 2)
        {
            system("cls");
            decode();
        }
        else if(dec == 3)
        {           
            system("cls");
        }
        else if(dec == 4)
        {
            system("cls");
        }
        else if(dec == 5)
        {
            system("cls");
            flag = false;
            break;
        }
        system("cls");
        dec = menu();
    }
    system("cls");
    return 0;
}

void encode()
{
//Variable declaration
    string image_name, text_name, full_text, temp_line;
    int last_used_row=0, counter=0,current_char=0,total_pixels_to_use,chars_per_row;
    int total_rows_needed,current_line, Desea, dec_image_1, info_image;
    bool files = true;
    cool_char pixel1R, pixel1G, pixel1B, pixel2R, pixel2G, pixel2B,
              pixel3R, pixel3G, temp_char;

    //User Interface
    pereza();
    cout << "      Ingrese el nombre de la imagen que quiere usar como portador" << endl;
    cout << "(incluya la extension, Imagenes en formato PNG, JPG/JPEG, BMP solamente)" << endl<<endl;
    cin>>image_name; //= "shyv.png";

    //Read image and upload it to memory
    CImg<unsigned char> image;
    image.load(image_name.c_str());
    CImg<unsigned char>* image_ptr = &image;
    //cout<<image.height()<<" "<<image.width()<<endl;

    //Total amount of pixels that can be used of the image, per row
    chars_per_row = image.width()/3;
    total_pixels_to_use = chars_per_row*3;

    //Ask the user to display the original image
    std::cout << "Desea ver la imagen:" << std::endl;
    std::cout << "  1.) Si" << std::endl;
    std::cout << "  2.) No" << std::endl;
    std::cin >> dec_image_1;
    if(dec_image_1 == 1)
    {
        //image.display();
        system("cls");
    }

    while(files)
    {
        //Clearing all the strings
        text_name.clear();
        full_text.clear();
        temp_line.clear();
        current_char = 0;
        //cout <<"Linea Actual: " <<last_used_row << endl;

        pereza();
        //User Interface
        cout << "Ingrese el nombre del texto a ocultar (incluya la extension)" << endl;
        cin>>text_name;// = "kek.txt";

        //Loading Text file to hide
        ifstream texto;

        texto.open(text_name,ifstream::in);
        text_name.erase(0,4);
        if (texto.good())
        {
            while (getline(texto,temp_line))
            {
                full_text += temp_line;
                full_text.push_back('\n');
            }
            texto.close();

            //Add the flag with the name of the text
            full_text = add_flag(full_text, text_name);

            //How many rows are neede to hide the current file
            total_rows_needed = (full_text.size()/chars_per_row)+1;

            //         Information about how will the file affect the image
            std::cout << "Desea ver informacion acerca de como el texto modfigicara la imagen:" << std::endl;
            std::cout << "  1.) Si" << std::endl;
            std::cout << "  2.) No" << std::endl;
            std::cin >> info_image;
            pereza();
            if (info_image == 1)
            {
                cout <<"Cantidad total de caracteres del texto: " << full_text.size()<< endl;
                cout <<"Lineas totales por el txt actual: "<< total_rows_needed << endl;
                cout <<"Ultima linea que se uso de la imagen: "<< last_used_row << endl;
                cout <<"Lineas disponibles para usar en la imagen: "<< (((image.height()) - last_used_row)) << endl;
            }
            if( (((image.height()) - last_used_row)) < total_rows_needed)
            {
                cout << "El archivo ocupa mas espacio del que esta disponible para ocultar en la imagen" << endl;
                _getch();
            }
            else
            {
                total_rows_needed = total_rows_needed + last_used_row;
                //cout <<"Cantidad Total de lineas con las que acabara el archivo: "<< total_rows_needed << endl;
                //cout <<"ultima linea usada: "<<last_used_row << endl;
                current_char = 0;
                for(last_used_row; last_used_row < total_rows_needed; last_used_row++)
                {
                    cout << ".";
                    for(int i=0; i<total_pixels_to_use; i++)
                    {
                        if(current_char < full_text.size())
                        {
                            temp_char.letra = full_text[current_char];

                            /*The switch lets us modify the pixels in order
                              without skipping*/
                            switch (counter)
                            {
                                //first 3 LSB are overwritten over the first pixel
                                case 0:
                                    {
                                        pixel1R.letra = image(i,last_used_row,0,0);
                                        pixel1G.letra = image(i,last_used_row,0,1);
                                        pixel1B.letra = image(i,last_used_row,0,2);

                                        pixel1R.bits.b8 = temp_char.bits.b1;
                                        pixel1G.bits.b8 = temp_char.bits.b2;
                                        pixel1B.bits.b8 = temp_char.bits.b3;

                                        image(i,last_used_row,0,0) = pixel1R.letra ;
                                        image(i,last_used_row,0,1) = pixel1G.letra ;
                                        image(i,last_used_row,0,2) = pixel1B.letra ;

                                        counter++;
                                    }
                                    break;
                                //first 3 LSB are overwritten over the first pixel
                                case 1:
                                    {
                                        pixel2R.letra = image(i,last_used_row,0,0);
                                        pixel2G.letra = image(i,last_used_row,0,1);
                                        pixel2B.letra = image(i,last_used_row,0,2);

                                        pixel2R.bits.b8 = temp_char.bits.b4;
                                        pixel2G.bits.b8 = temp_char.bits.b5;
                                        pixel2B.bits.b8 = temp_char.bits.b6;

                                        image(i,last_used_row,0,0) = pixel2R.letra ;
                                        image(i,last_used_row,0,1) = pixel2G.letra ;
                                        image(i,last_used_row,0,2) = pixel2B.letra ;

                                        counter++;
                                    }
                                    break;
                                case 2:
                                    {
                                        pixel3R.letra = image(i,last_used_row,0,0);
                                        pixel3G.letra = image(i,last_used_row,0,1);

                                        pixel3G.bits.b8 = temp_char.bits.b8;
                                        pixel3R.bits.b8 = temp_char.bits.b7;

                                        image(i,last_used_row,0,0) = pixel3R.letra ;
                                        image(i,last_used_row,0,1) = pixel3G.letra ;

                                        current_char++;
                                        counter = 0;
                                    }
                                    break;
                            }
                        }
                    }
                    counter = 0;
                }
            }
            cout << endl;
            system("cls");
            //image.display();
            system("cls");
            /*  CImgDisplay main_disp(image,"Click a point");
              main_disp.wait(1000);*/

            cout << "Desea incluir mas archivos?" << endl;
            cout << "  1) Si" << endl;
            cout << "  2) No" << endl;
            cin >> Desea;
            system("cls");
            texto.close();
            if (Desea == 1)
            {
                files = true;
            }
            else
            {
                files = false;
            }
        }
        else
        {
            cout << "No se encontro el documento." << endl;
            texto.close();
        }
    }
    size_t i = image_name.find(".");
    //cout << image_name.substr(i) << endl;
    if(image_name.substr(i) == ".png")
        image.save_png("output.png",100);
    if(image_name.substr(i) == ".jpg" || image_name.substr(i) == ".jpeg")
        image.save_jpeg("output.jpg",100);
    if(image_name.substr(i) == ".bmp")
        image.save_bmp("output.bmp");
}

void decode()
{
    //declaration
    string full_text, coded_image, temp_string, file_name;
    bool flag = false;
    char letra;
    int cont_flag = 0, current_row=0, chars_per_row, counter =0 ,end_name, l = 14;
    cool_char pixel1R,pixel1G,pixel1B,
              pixel2R,pixel2G,pixel2B,
              pixel3R,pixel3G,temp_char;

    //Image wi
    pereza();
    cout << "Ingrese el nombre de la imagen (incluya la extension)" << endl;
    cin >> coded_image;

    CImg<unsigned char> image;
    image.load(coded_image.c_str());
    CImg<unsigned char>* image_ptr = &image;
    cout<<image.height()<<" "<<image.width()<<endl;

    chars_per_row = image.width()/3;
    chars_per_row *= 3;

    if (check(image_ptr,current_row))
    {
        cout << check(image_ptr,l)  << endl;
        while (check(image_ptr,current_row))
        {
            cont_flag = 0;
            while (cont_flag != 3)
            {
                for(int i=0; i<chars_per_row; i++)
                {
                    if(cont_flag != 3)
                    {
                        switch (counter)
                        {
                            case 0:
                                {
                                    pixel1R.letra = image(i,current_row,0,0);
                                    pixel1G.letra = image(i,current_row,0,1);
                                    pixel1B.letra = image(i,current_row,0,2);
                                    counter++;
                                }
                                break;
                            case 1:
                                {
                                    pixel2R.letra = image(i,current_row,0,0);
                                    pixel2G.letra = image(i,current_row,0,1);
                                    pixel2B.letra = image(i,current_row,0,2);
                                    counter++;
                                }
                                break;
                            case 2:
                                {
                                    pixel3R.letra = image(i,current_row,0,0);
                                    pixel3G.letra = image(i,current_row,0,1);

                                    temp_char.bits.b1 = pixel1R.bits.b8;
                                    temp_char.bits.b2 = pixel1G.bits.b8;
                                    temp_char.bits.b3 = pixel1B.bits.b8;
                                    temp_char.bits.b4 = pixel2R.bits.b8;
                                    temp_char.bits.b5 = pixel2G.bits.b8;
                                    temp_char.bits.b6 = pixel2B.bits.b8;
                                    temp_char.bits.b8 = pixel3G.bits.b8;
                                    temp_char.bits.b7 = pixel3R.bits.b8;

                                    full_text += temp_char.letra;
                                    temp_string += temp_char.letra;

                                    if(temp_string == "!z!")
                                    {
                                        if (cont_flag == 1)
                                            end_name = full_text.size()-6;

                                        cont_flag++;
                                    }
                                    if(temp_string.size() == 3)
                                    {
                                        temp_string.erase(temp_string.begin()) ;
                                    }
                                    counter = 0;
                                }
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                current_row++;
            }
            file_name = full_text.substr(3,end_name);
            full_text.erase(full_text.begin(),full_text.begin()+(end_name+6));
            full_text.erase(full_text.end()-3,full_text.end());
            //cout << full_text << endl;
            cout << file_name << endl;
            ofstream newfile(file_name, ofstream::out);
            if(newfile.is_open())
            {
                newfile << full_text;
                newfile.close();
            }
            else
            {
                cout << "No se pudo crear el archivo." << endl;
            }
            full_text.clear();
            temp_string.clear();
            file_name.clear();
        }
        cout << "No hay mas archivos para Extraer, revise los archivos en la carpeta" << endl;
    }
    else
    {
        cout << "La imagen no contiene ningun documento oculto" << endl;
    }
}

string add_flag(string& original,string& name)
{
    string temp = "!z!"+name+"!z!";
    temp += original;
    temp.pop_back();
    temp += "!z!";
    return temp;
}

bool check(CImg<unsigned char>* ptr,int& line)
{
    int counter=0;
    bool flag, even = true;
    cool_char pixel1R,pixel1G,pixel1B,
              pixel2R,pixel2G,pixel2B,
              pixel3R,pixel3G,temp_char;
    for(int i=0; i<9; i++)
    {
        switch (counter)
        {
            case 0:
                {
                    pixel1R.letra = (*ptr)(i,line,0,0);
                    pixel1G.letra = (*ptr)(i,line,0,1);
                    pixel1B.letra = (*ptr)(i,line,0,2);
                    temp_char.bits.b1 = pixel1R.bits.b8;
                    temp_char.bits.b2 = pixel1G.bits.b8;
                    temp_char.bits.b3 = pixel1B.bits.b8;
                    counter++;
                }
                break;
            case 1:
                {
                    pixel2R.letra = (*ptr)(i,line,0,0);
                    pixel2G.letra = (*ptr)(i,line,0,1);
                    pixel2B.letra = (*ptr)(i,line,0,2);
                    temp_char.bits.b4 = pixel2R.bits.b8;
                    temp_char.bits.b5 = pixel2G.bits.b8;
                    temp_char.bits.b6 = pixel2B.bits.b8;
                    counter++;
                }
                break;
            case 2:
                {
                    pixel3R.letra = (*ptr)(i,line,0,0);
                    pixel3G.letra = (*ptr)(i,line,0,1);
                    temp_char.bits.b7 = pixel3R.bits.b8;
                    temp_char.bits.b8 = pixel3G.bits.b8;
                    if(even)
                    {
                        if (temp_char.letra != '!')
                            return false;
                        else
                            even = false;
                    }
                    else
                    {
                        if (temp_char.letra != 'z')
                            return false;
                        else
                            even = true;
                    }
                    counter = 0;
                }
                break;
        }
    }
    return true;
}

void pereza()
{
    std::cout << endl << std::endl;
}
