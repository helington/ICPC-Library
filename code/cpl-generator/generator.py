import os
import itertools
from pathlib import Path
from collections import defaultdict
from datetime import date

from fpdf import FPDF
from pygments.lexers import CppLexer
from pygments.token import Token

# --- PDF Class with Smart Open/Close Boxes & Footer ---

class PDFColumns(FPDF):
    def __init__(self):
        super().__init__()
        self.current_col = 0
        self.col_width = 92
        self.gap = 6
        self.top_margin = 10
        self.col_start_y = 10
        self.use_columns = False
        self.code_font = 'courier'
        
        # Box tracking state variables
        self.in_box = False
        self.box_start_y = 0
        self.box_has_top = True
        self.code_padding = 0

    def footer(self):
        self.set_y(-10)
        self.set_font(self.code_font, '', 8)
        self.set_text_color(0, 0, 0)
        self.set_x(0)
        self.cell(210, 10, f'{self.page_no()}', align='C')

    def set_column(self, col_index):
        if not self.use_columns:
            return
            
        self.current_col = col_index
        x_position = 10 + col_index * (self.col_width + self.gap)
        
        self.set_left_margin(x_position + self.code_padding)
        self.set_x(x_position + self.code_padding)
        self.set_right_margin(210 - (x_position + self.col_width) + self.code_padding)

    def accept_page_break(self):
        if self.use_columns:
            return False
        return super().accept_page_break()

    def start_box(self):
        self.in_box = True
        self.box_start_y = self.get_y()
        self.box_has_top = True

    def draw_box_borders(self, is_bottom_closed):
        if not self.in_box:
            return
            
        x_pos = 10 + self.current_col * (self.col_width + self.gap)
        y_start = self.box_start_y
        y_end = self.get_y()

        self.set_draw_color(0, 0, 0)
        self.set_line_width(0.2)

        # Borders
        self.line(x_pos, y_start, x_pos, y_end)
        self.line(x_pos + self.col_width, y_start, x_pos + self.col_width, y_end)
        
        if self.box_has_top:
            self.line(x_pos, y_start, x_pos + self.col_width, y_start)
            
        if is_bottom_closed:
            self.line(x_pos, y_end, x_pos + self.col_width, y_end)

    def end_box(self):
        if self.in_box:
            self.draw_box_borders(is_bottom_closed=True)
            self.in_box = False

    def check_space(self, required_height):
        if not self.use_columns:
            return
            
        if self.get_y() + required_height > 285:
            if self.in_box:
                self.draw_box_borders(is_bottom_closed=False)
            
            if self.current_col == 0:
                self.set_column(1)
                self.set_y(self.col_start_y)
            else:
                self.add_page()
                self.col_start_y = self.top_margin
                self.set_column(0)
                self.set_y(self.col_start_y)
                
            if self.in_box:
                self.box_start_y = self.get_y()
                self.box_has_top = False

# --- Code Parsing & Highlighting ---

def get_file_content(path):
    try:
        with open(path, 'r', encoding='utf-8') as file:
            lines = file.read().splitlines()
            
        cleaned_lines = []
        prev_empty = False
        
        for line in lines:
            is_empty = not line.strip()
            if is_empty and prev_empty:
                continue
            cleaned_lines.append(line)
            prev_empty = is_empty
            
        return '\n'.join(cleaned_lines)
    except Exception as e:
        return f'//> Error reading file: {e}\n'

def write_colored_code(pdf, lexer, content):
    tokens = lexer.get_tokens(content)
    
    # Font and spacing settings
    code_size = 6.0
    line_height = 3.0
    
    # Token style mapping
    token_colors = {
        Token.Comment: (146, 131, 116),
        Token.Comment.Preproc: (143, 63, 113),
        Token.Keyword: (157, 0, 6),
        Token.String: (121, 116, 14),
        Token.Name.Function: (7, 102, 120),
        Token.Number: (66, 123, 88),
        Token.Operator: (175, 58, 3)
    }

    for token_type, token_string in tokens:
        # Default color: Dark Blue / Black
        r, g, b = 20, 20, 30
        
        # Find color based on token type hierarchy
        for ttype, color in token_colors.items():
            if ttype in token_type:
                r, g, b = color
                break

        pdf.set_font(pdf.code_font, size=code_size)
        pdf.set_text_color(r, g, b)
        
        parts = token_string.split('\n')
        for i, part in enumerate(parts):
            if part:
                pdf.write(h=line_height, text=part)
            if i < len(parts) - 1:
                pdf.ln(line_height)
                pdf.check_space(line_height)

# --- Modular Builders ---

def draw_toc(pdf, toc_data, user_info, page_offset=0):
    pdf.add_page()
    pdf.use_columns = False
    
    y_start = pdf.get_y()
    icon_path = user_info.get('icon_path')
    
    if icon_path and os.path.exists(icon_path):
        pdf.image(icon_path, x=80, y=y_start, w=50)
        pdf.set_y(y_start + 32)
        
    pdf.set_font(pdf.code_font, 'B', 16)
    pdf.set_text_color(40, 44, 52)
    pdf.cell(0, 6, 'COMPETITIVE PROGRAMMING LIBRARY', align='C', new_x='LMARGIN', new_y='NEXT')
    
    pdf.set_font(pdf.code_font, '', 10)
    pdf.set_text_color(60, 60, 60)
    name_inst = f"{user_info.get('name', '')} | {user_info.get('team', '')}"
    pdf.cell(0, 5, name_inst, align='C', new_x='LMARGIN', new_y='NEXT')
    
    pdf.set_font(pdf.code_font, '', 8)
    pdf.cell(0, 4, f'Compiled on: {date.today()}', align='C', new_x='LMARGIN', new_y='NEXT')
    
    pdf.ln(6)
    pdf.set_font(pdf.code_font, 'B', 12)
    pdf.cell(0, 6, 'INDEX', align='C', new_x='LMARGIN', new_y='NEXT')
    pdf.ln(2)
    
    pdf.use_columns = True
    pdf.col_start_y = pdf.get_y()
    pdf.set_column(0)
    
    for category, items in toc_data.items():
        pdf.check_space(8)
        pdf.set_font(pdf.code_font, 'B', 9)
        pdf.set_text_color(220, 50, 47)
        pdf.cell(pdf.col_width, 5, category.upper(), new_x='LMARGIN', new_y='NEXT')
        
        pdf.set_font(pdf.code_font, '', 7.5)
        pdf.set_text_color(60, 60, 60)
        
        for name, page in items:
            pdf.check_space(3.0)
            display_name = f" {name} "
            dots = max(0, 52 - len(display_name))
            line = f"{display_name}{'.' * dots} {page + page_offset:02d}"
            pdf.cell(pdf.col_width, 3.0, line, new_x='LMARGIN', new_y='NEXT')
        pdf.ln(2)

def draw_algorithms(pdf, library_data, lexer, save_toc=False):
    toc_data = defaultdict(list)
    pdf.add_page()
    pdf.use_columns = True
    pdf.col_start_y = pdf.top_margin
    pdf.set_column(0)

    for category_name, files in sorted(library_data.items()):
        pdf.check_space(10)
        pdf.ln(2)
        pdf.set_font(pdf.code_font, 'B', 10)
        pdf.set_text_color(220, 50, 47)
        pdf.cell(pdf.col_width, 6, category_name.upper(), align='L', new_x='LMARGIN', new_y='NEXT')
        pdf.ln(1)

        for item in sorted(files, key=lambda x: x['name']):
            pdf.check_space(12)
            if save_toc:
                toc_data[category_name].append((item['name'], pdf.page_no()))

            pdf.start_box()
            pdf.set_font(pdf.code_font, 'B', 7.5)
            pdf.set_fill_color(0, 0, 0)
            pdf.set_text_color(255, 255, 255)
            pdf.cell(pdf.col_width, 5, f" {item['name']}", fill=True, new_x='LMARGIN', new_y='NEXT')
            
            pdf.set_y(pdf.get_y() + 1)
            pdf.code_padding = 1.5
            pdf.set_column(pdf.current_col)
            
            write_colored_code(pdf, lexer, item['content'])
            
            if pdf.get_x() > pdf.l_margin + 0.1:
                pdf.ln(3.0)
                
            pdf.set_y(pdf.get_y() + 1)
            pdf.code_padding = 0
            pdf.set_column(pdf.current_col)
            pdf.end_box()
            pdf.ln(4)
            
    return toc_data

# --- Main Flow ---

def scan_library_files():
    # Caminho focado na sua pasta de funções
    root_directory = Path(__file__).resolve().parent.parent / "code"
    categorized_files = defaultdict(list)
    
    # Verificação de segurança: se a pasta não existir, o script avisa em vez de travar
    if not root_directory.exists():
        print(f"❌ Erro: A pasta '{root_directory}' não foi encontrada!")
        return categorized_files

    # Criamos uma lista com as extensões que você quer incluir
    for ext in ['*.cpp', '*.txt']:
        # O rglob vai buscar recursivamente tanto .cpp quanto .txt
        for path in root_directory.rglob(ext):
            category = str(path.parent.relative_to(root_directory)).replace('\\', '/')
            
            # Se o arquivo estiver solto na FuncRepositoryCpp, chamamos de 'Geral'
            if category == '.':
                category = 'Geral'
                
            categorized_files[category].append({
                'name': path.name,
                'content': get_file_content(path)
            })
            
    return categorized_files

def generate_pdf(library_data, user_info, filename='library.pdf'):
    lexer = CppLexer()

    # Pass 1 & 2: Calculate TOC (Table of Contents)
    dummy = PDFColumns()
    dummy.set_margins(10, 10, 10)
    dummy.set_auto_page_break(True, margin=2)
    raw_toc = draw_algorithms(dummy, library_data, lexer, save_toc=True)
    
    dummy_toc = PDFColumns()
    dummy_toc.set_margins(10, 10, 10)
    dummy_toc.set_auto_page_break(True, margin=2)
    draw_toc(dummy_toc, raw_toc, user_info, page_offset=0)
    toc_pages = dummy_toc.page_no()

    # Final Pass
    pdf = PDFColumns()
    pdf.alias_nb_pages()
    pdf.set_margins(10, 10, 10)
    pdf.set_auto_page_break(True, margin=2)
    
    draw_toc(pdf, raw_toc, user_info, page_offset=toc_pages)
    draw_algorithms(pdf, library_data, lexer, save_toc=False)

    pdf.output(filename)
    print(f'> Success! {filename} generated.')

if __name__ == '__main__':
    lib_data = scan_library_files()
    if lib_data:
        user_info = {
            'name': 'Helington Willamy',
            'team': 'None',
            'icon_path': 'None'
        }
        generate_pdf(lib_data, user_info)