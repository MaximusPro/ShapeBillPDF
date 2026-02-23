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
