# cmake/GenerateBalls.cmake Utilitários para gerar PNGs de bolas com ImageMagick
# (convert). Fornece: - generate_default_ball() - gen_ball(<name> <R,G,B>
# <size_px>) - setup_ball_assets_target(<target>)
#
# Uso típico: include(cmake/GenerateBalls.cmake) generate_default_ball() # cria
# ${binary_dir}/ball.png # ou várias: # gen_ball(ball_blue  "30,144,255" 128) #
# gen_ball(ball_red   "220,20,60"  128) setup_ball_assets_target(app) # copia os
# PNGs p/ pasta do executável

# ---------- Opções padrão (para generate_default_ball) ----------
set(BALL_SIZE_PX
    128
    CACHE STRING "Ball diameter (px)")
set(BALL_COLOR
    "30,144,255"
    CACHE STRING "Ball RGB color as R,G,B")

# ---------- Detecta ImageMagick (magick convert ou convert) ----------
find_program(IMAGEMAGICK_MAGICK NAMES magick)
find_program(IMAGEMAGICK_CONVERT NAMES convert)

if(IMAGEMAGICK_MAGICK)
  set(_IM_CMD ${IMAGEMAGICK_MAGICK} convert)
elseif(IMAGEMAGICK_CONVERT)
  set(_IM_CMD ${IMAGEMAGICK_CONVERT})
else()
  message(
    FATAL_ERROR
      "ImageMagick não encontrado. Instale 'imagemagick' (magick/convert).")
endif()

# Guardamos a lista global de imagens geradas
set_property(GLOBAL PROPERTY BALL_IMAGES "")

# Helper interno: adiciona arquivo à lista global
function(_ball__append_output _file)
  get_property(_imgs GLOBAL PROPERTY BALL_IMAGES)
  list(APPEND _imgs "${_file}")
  set_property(GLOBAL PROPERTY BALL_IMAGES "${_imgs}")
endfunction()

# ---------- API: gen_ball(name color size_px) ----------
# Gera <name>.png em ${CMAKE_CURRENT_BINARY_DIR} com fundo transparente.
function(gen_ball name color size_px)
  if(ARGC LESS 3)
    message(FATAL_ERROR "gen_ball(name color size_px) requer 3 argumentos.")
  endif()

  set(_out "${CMAKE_CURRENT_BINARY_DIR}/${name}.png")

  # Raio = size/2 - 4; centro = size/2  (avaliados como INTEIROS)
  math(EXPR _radius "(${size_px} / 2) - 4")
  math(EXPR _center "${size_px} / 2")
  # Ponto na circunferência: mesmo x do centro e y deslocado pelo raio
  math(EXPR _x1 "${_center}")
  math(EXPR _y1 "${_center} - ${_radius}")

  add_custom_command(
    OUTPUT "${_out}"
    COMMAND
      ${_IM_CMD}
      -size ${size_px}x${size_px} xc:none
      -fill "rgb(${color})"
      -stroke "rgb(${color})" -strokewidth 1
      -draw "circle ${_center},${_center} ${_x1},${_y1}"
      "${_out}"
    COMMENT "Gerando ${name}.png (${size_px}px, rgb(${color}))"
    VERBATIM)

  # Devolve o caminho por conveniência (opcional)
  set(${name}_PNG
      "${_out}"
      PARENT_SCOPE)

  # Adiciona à lista global
  _ball__append_output("${_out}")
endfunction()

# ---------- API: generate_default_ball() ----------
# Atalho que gera "ball.png" com BALL_COLOR/BALL_SIZE_PX
function(generate_default_ball)
  gen_ball(ball "${BALL_COLOR}" "${BALL_SIZE_PX}")
  # expõe BALL_PNG ao chamador
  set(BALL_PNG
      "${CMAKE_CURRENT_BINARY_DIR}/ball.png"
      PARENT_SCOPE)
endfunction()

# ---------- API: setup_ball_assets_target(target) ----------
# Cria target "assets" (ALL) que depende dos PNGs gerados, garante dependência
# do executável, copia p/ pasta de runtime, e registra instalação dos arquivos.
function(setup_ball_assets_target target)
  if(NOT TARGET ${target})
    message(
      FATAL_ERROR "setup_ball_assets_target: alvo '${target}' não existe.")
  endif()

  get_property(_imgs GLOBAL PROPERTY BALL_IMAGES)
  if(NOT _imgs)
    message(
      FATAL_ERROR
        "Nenhuma bola foi registrada. Chame generate_default_ball() ou gen_ball(...) antes de setup_ball_assets_target()."
    )
  endif()

  # Gera todos os assets no build
  add_custom_target(assets ALL DEPENDS ${_imgs})

  # Garante que o executável dependa de 'assets'
  add_dependencies(${target} assets)

  # Copia os PNGs p/ diretório do binário (p/ runtime)
  add_custom_command(
    TARGET ${target}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${_imgs}
            $<TARGET_FILE_DIR:${target}>
    COMMENT "Copiando bolas PNG para a pasta do executável")

  # Instala junto do app (opcional)
  install(FILES ${_imgs} DESTINATION .)
endfunction()

