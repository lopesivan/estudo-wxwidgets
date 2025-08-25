#!/usr/bin/env python3
"""
Converte YAML para XML no formato específico do makefile
Uso: python yaml_to_xml.py makefile.yaml > makefile.xml
"""

import yaml
import sys
from xml.etree.ElementTree import Element, SubElement, tostring
from xml.dom import minidom

def create_xml_from_yaml(data):
    """Converte dados YAML para estrutura XML específica do makefile"""
    root = Element('makefile')
    
    makefile_data = data.get('makefile', {})
    
    # Include
    if 'include' in makefile_data:
        include = SubElement(root, 'include')
        include.set('file', makefile_data['include']['file'])
    
    # Conditions
    if 'conditions' in makefile_data:
        for condition in makefile_data['conditions']:
            if_elem = SubElement(root, 'if')
            if_elem.set('cond', condition['if'])
            
            if 'set' in condition:
                for key, value in condition['set'].items():
                    set_elem = SubElement(if_elem, 'set')
                    set_elem.set('var', key)
                    set_elem.text = str(value)
    
    # Variables
    if 'variables' in makefile_data:
        for var_name, var_data in makefile_data['variables'].items():
            set_elem = SubElement(root, 'set')
            set_elem.set('var', var_name)
            
            if isinstance(var_data, dict) and 'if' in var_data:
                if_elem = SubElement(set_elem, 'if')
                if_elem.set('cond', var_data['if'])
                if_elem.text = var_data.get('value', '')
            else:
                set_elem.text = str(var_data)
    
    # Executables
    if 'executables' in makefile_data:
        for exe_name, exe_data in makefile_data['executables'].items():
            exe_elem = SubElement(root, 'exe')
            exe_elem.set('id', exe_name)
            exe_elem.set('template', exe_data.get('template', ''))
            exe_elem.set('template_append', exe_data.get('template_append', ''))
            exe_elem.set('cond', exe_data.get('condition', ''))
            
            # Sources
            if 'sources' in exe_data:
                sources_elem = SubElement(exe_elem, 'sources')
                sources_elem.text = '\n            ' + '\n            '.join(exe_data['sources']) + '\n        '
            
            # Headers  
            if 'headers' in exe_data:
                headers_elem = SubElement(exe_elem, 'headers')
                headers_elem.text = '\n            ' + '\n            '.join(exe_data['headers']) + '\n        '
            
            # Libraries
            if 'libraries' in exe_data:
                libs = exe_data['libraries']
                
                # wx libraries
                if 'wx' in libs:
                    for lib in libs['wx']:
                        wx_lib = SubElement(exe_elem, 'wx-lib')
                        wx_lib.text = lib
                
                # sys libraries
                if 'sys' in libs:
                    for lib in libs['sys']:
                        sys_lib = SubElement(exe_elem, 'sys-lib')
                        sys_lib.text = lib
    
    return root

def prettify_xml(elem):
    """Retorna XML formatado de forma legível"""
    rough_string = tostring(elem, 'unicode')
    reparsed = minidom.parseString(rough_string)
    return reparsed.toprettyxml(indent="    ")[23:]  # Remove primeira linha

def main():
    if len(sys.argv) != 2:
        print("Uso: python yaml_to_xml.py <arquivo.yaml>", file=sys.stderr)
        sys.exit(1)
    
    yaml_file = sys.argv[1]
    
    try:
        with open(yaml_file, 'r') as f:
            data = yaml.safe_load(f)
        
        xml_root = create_xml_from_yaml(data)
        
        # Adiciona declaração XML
        print('<?xml version="1.0" ?>')
        print(prettify_xml(xml_root).strip())
        
    except FileNotFoundError:
        print(f"Erro: Arquivo '{yaml_file}' não encontrado", file=sys.stderr)
        sys.exit(1)
    except yaml.YAMLError as e:
        print(f"Erro ao processar YAML: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()
