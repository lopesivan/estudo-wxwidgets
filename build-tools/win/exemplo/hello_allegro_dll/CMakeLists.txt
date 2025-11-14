cmake_minimum_required(VERSION 3.10)
project(allegro_app)

# Define o caminho base do Allegro
set(ALLEGRO_ROOT "C:/Users/ivan/source/repos/SDKs/MinGW64/allegro")

# Define manualmente os caminhos de include e lib
set(ALLEGRO_INCLUDE_DIR "${ALLEGRO_ROOT}/include")
set(ALLEGRO_LIB_DIR "${ALLEGRO_ROOT}/lib")

# Cria o executável
add_executable(allegro_app src/main.cpp)

# Adiciona os diretórios de include
target_include_directories(allegro_app PRIVATE ${ALLEGRO_INCLUDE_DIR})

# Adiciona o diretório de bibliotecas
target_link_directories(allegro_app PRIVATE ${ALLEGRO_LIB_DIR})

# Linka com as bibliotecas do Allegro
# Para versão dynamic (DLL), use os nomes sem o sufixo -static
target_link_libraries(allegro_app 
    allegro
    allegro_primitives
    allegro_font
    allegro_ttf
)

# Se você baixou a versão STATIC, descomente as linhas abaixo e comente as de cima:
# target_link_libraries(exemplo 
#     allegro-static
#     allegro_primitives-static
# )

# Adicionar mais addons conforme necessário:
# allegro_image, allegro_font, allegro_ttf, allegro_audio, allegro_acodec, etc.

