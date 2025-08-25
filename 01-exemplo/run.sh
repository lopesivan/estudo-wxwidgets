#!/usr/bin/env bash

BAKEFILE=/home/ivan/wx/linux-wx-3.2.4/bin/bakefile

# Instalar dependência
#pip install PyYAML

# Converter
python yaml_to_xml.py makefile.yaml >makefile.xml

echo PYENV_VERSION=2.7.18 $BAKEFILE -f gnu makefile.xml
PYENV_VERSION=2.7.18 $BAKEFILE -f gnu makefile.xml
