# Russian VAT Invoice PDF Generator

A simple console utility written in C++ for Linux that helps quickly create **Russian tax invoices (счёт-фактура)** in PDF format.

The program asks for invoice details interactively in the terminal (or uses default demo values), generates a LaTeX file with the standard Russian invoice layout, and automatically compiles it into PDF using `pdflatex`.

**Current demo date in the code:** February 23, 2026  
**Form compliance:** Based on Russian Government Decree No. 1137 (Appendix No. 1) as amended up to 2025–2026.

## Features

- Interactive input of all main invoice fields
- Sensible default values — just press Enter to test quickly
- Support for multiple line items (goods / services)
- Automatic calculation of totals: excluding VAT, VAT amount, grand total
- Beautiful PDF output via LaTeX with proper Cyrillic / Russian support
- Minimal dependencies — only standard C++ library + `pdflatex`

## Requirements

- Linux (tested on Ubuntu 22.04 / 24.04, Debian, Fedora, etc.)
- g++ compiler with C++17 support
- TeX Live distribution with Russian language support:

```bash
# Ubuntu / Debian
sudo apt update
sudo apt install texlive-full texlive-lang-cyrillic

# Fedora
sudo dnf install texlive-scheme-full texlive-collection-langcyrillic
Installation

Clone the repository (or just download invoice_gen.cpp)

Bash
git clone https://github.com/YOUR-USERNAME/russian-invoice-pdf-generator.git
cd russian-invoice-pdf-generator

Compile

Bash
g++ -std=c++17 -Wall -O2 invoice_gen.cpp -o invoice-gen

(Optional) Make it globally available

Bash
sudo cp invoice-gen /usr/local/bin/
Usage
Bash
./invoice-gen
The program will ask step-by-step:

Invoice number and date
Seller / Buyer / Shipper / Consignee details
Payment document reference (if any)
Goods / services (you can add several positions)

Press Enter on any question to use the built-in demo values.
After you finish entering data:

invoice.tex file is created
pdflatex is called automatically → invoice.pdf appears
Open the resulting PDF in any viewer (evince, okular, firefox, etc.)

Quick test with defaults
Bash
./invoice-gen
# just press Enter on every prompt
