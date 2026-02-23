#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>  // для дат
#include <ctime>    // для текущей даты

struct Item {
    std::string name;       // 1: Наименование
    std::string unit_code;  // 2: код единицы
    std::string unit_name;  // 2a: обозначение
    double quantity;        // 3: Количество
    double price;           // 4: Цена за единицу
    double total_no_tax;    // 5: Стоимость без налога (вычисляется)
    std::string excise;     // 6: Акциз (обычно "без акциза")
    std::string tax_rate;   // 7: Ставка налога (e.g. "20%")
    double tax_amount;      // 8: Сумма налога (вычисляется)
    double total_with_tax;  // 9: Стоимость с налогом (вычисляется)
    std::string country_code; // 10: Код страны
    std::string country_name; // 10a: Наименование страны
    std::string customs_decl; // 11: Номер декларации (обычно "-")
};

void generate_latex(const std::string& number, const std::string& date,
                    const std::string& seller, const std::string& seller_addr, const std::string& seller_inn_kpp,
                    const std::string& shipper, const std::string& consignee,
                    const std::string& payment_doc,
                    const std::string& buyer, const std::string& buyer_addr, const std::string& buyer_inn_kpp,
                    const std::vector<Item>& items) {
    std::ofstream tex_file("invoice.tex");
    if (!tex_file) {
        std::cerr << "Ошибка создания invoice.tex" << std::endl;
        return;
    }

    // LaTeX преамбула для русского и таблицы
    tex_file << "\\documentclass[a4paper,12pt]{article}\n"
             << "\\usepackage[utf8]{inputenc}\n"
             << "\\usepackage[T2A]{fontenc}\n"
             << "\\usepackage[russian]{babel}\n"
             << "\\usepackage{geometry}\n"
             << "\\geometry{margin=1cm}\n"
             << "\\usepackage{multirow}\n"
             << "\\usepackage{longtable}\n"
             << "\\begin{document}\n"
             << "\\centering\n"
             << "\\textbf{СЧЕТ-ФАКТУРА № " << number << " от " << date << "}\n\n"
             << "Продавец: " << seller << "\\\\\n"
             << "Адрес: " << seller_addr << "\\\\\n"
             << "ИНН/КПП продавца: " << seller_inn_kpp << "\\\\\n"
             << "Грузоотправитель и его адрес: " << shipper << "\\\\\n"
             << "Грузополучатель и его адрес: " << consignee << "\\\\\n"
             << "К платежно-расчетному документу: " << payment_doc << "\\\\\n"
             << "Покупатель: " << buyer << "\\\\\n"
             << "Адрес: " << buyer_addr << "\\\\\n"
             << "ИНН/КПП покупателя: " << buyer_inn_kpp << "\\\\\n"
             << "Валюта: Российский рубль, 643\\\\\n\n";

    // Таблица
    tex_file << "\\begin{longtable}{|p{4cm}|c|c|c|c|c|c|c|c|c|c|c|c|}\n"
             << "\\hline\n"
             << "\\multirow{2}{*}{Наименование товара (описание выполненных работ, оказанных услуг), имущественного права} & "
             << "\\multicolumn{2}{c|}{Единица измерения} & \\multirow{2}{*}{Количество (объем)} & \\multirow{2}{*}{Цена (тариф) за единицу измерения} & "
             << "\\multirow{2}{*}{Стоимость товаров (работ, услуг), имущественных прав без налога - всего} & \\multirow{2}{*}{В том числе сумма акциза} & "
             << "\\multirow{2}{*}{Налоговая ставка} & \\multirow{2}{*}{Сумма налога, предъявляемая покупателю} & "
             << "\\multirow{2}{*}{Стоимость товаров (работ, услуг), имущественных прав с налогом - всего} & "
             << "\\multicolumn{2}{c|}{Страна происхождения товара} & \\multirow{2}{*}{Номер таможенной декларации} \\\\ \\cline{2-3} \\cline{11-12}\n"
             << " & код & условное обозначение (национальное) &  &  &  &  &  &  &  & цифровой код & краткое наименование &  \\\\ \\hline\n"
             << "\\endhead\n";

    double grand_total_no_tax = 0, grand_tax = 0, grand_total_with_tax = 0;

    for (const auto& item : items) {
        tex_file << item.name << " & " << item.unit_code << " & " << item.unit_name << " & " << item.quantity << " & "
                 << item.price << " & " << item.total_no_tax << " & " << item.excise << " & " << item.tax_rate << " & "
                 << item.tax_amount << " & " << item.total_with_tax << " & " << item.country_code << " & "
                 << item.country_name << " & " << item.customs_decl << " \\\\ \\hline\n";

        grand_total_no_tax += item.total_no_tax;
        grand_tax += item.tax_amount;
        grand_total_with_tax += item.total_with_tax;
    }

    // Итог
    tex_file << "Всего к оплате & & & & & " << grand_total_no_tax << " & X & & " << grand_tax << " & " << grand_total_with_tax
             << " & & &  \\\\ \\hline\n"
             << "\\end{longtable}\n\n";

    // Подписи
    tex_file << "Руководитель организации или иное уполномоченное лицо: _______________ (подпись) _______________ (Ф.И.О.)\\\\\n"
             << "Индивидуальный предприниматель: _______________ (подпись) _______________ (Ф.И.О.)\\\\\n"
             << "Главный бухгалтер или иное уполномоченное лицо: _______________ (подпись) _______________ (Ф.И.О.)\\\\\n"
             << "\\end{document}\n";

    tex_file.close();

    // Компиляция
    int result = std::system("pdflatex -interaction=nonstopmode invoice.tex");
    if (result == 0) {
        std::cout << "PDF сгенерирован: invoice.pdf" << std::endl;
    } else {
        std::cerr << "Ошибка компиляции pdflatex. Убедись, что pdflatex установлен." << std::endl;
    }
}

std::string get_input(const std::string& prompt, const std::string& default_val) {
    std::string input;
    std::cout << prompt << " (по умолчанию: " << default_val << "): ";
    std::getline(std::cin, input);
    return input.empty() ? default_val : input;
}

double get_double(const std::string& prompt, double default_val) {
    std::string input;
    std::cout << prompt << " (по умолчанию: " << default_val << "): ";
    std::getline(std::cin, input);
    if (input.empty()) return default_val;
    return std::stod(input);
}

int main() {
    // Дефолтные значения (на моё усмотрение, на основе примера)
    std::string number = "1";
    std::tm time_in = {};
    time_in.tm_year = 126;  // 2026 - 1900
    time_in.tm_mon = 1;     // февраль (0-11)
    time_in.tm_mday = 23;
    std::ostringstream date_ss;
    date_ss << std::put_time(&time_in, "%d.%m.%Y");
    std::string date = date_ss.str();

    std::string seller = "ООО \"Пример Компани\"";
    std::string seller_addr = "г. Москва, ул. Ленина, д. 1";
    std::string seller_inn_kpp = "1234567890/123456789";
    std::string shipper = "То же, что и продавец";
    std::string consignee = "То же, что и покупатель";
    std::string payment_doc = "№ 1 от 23.02.2026";
    std::string buyer = "ИП Иванов И.И.";
    std::string buyer_addr = "г. Санкт-Петербург, пр. Невский, д. 2";
    std::string buyer_inn_kpp = "0987654321/098765432";

    // Интерактивный ввод
    number = get_input("Номер счёт-фактуры", number);
    date = get_input("Дата (ДД.ММ.ГГГГ)", date);
    seller = get_input("Продавец", seller);
    seller_addr = get_input("Адрес продавца", seller_addr);
    seller_inn_kpp = get_input("ИНН/КПП продавца", seller_inn_kpp);
    shipper = get_input("Грузоотправитель", shipper);
    consignee = get_input("Грузополучатель", consignee);
    payment_doc = get_input("К платежно-расчетному документу", payment_doc);
    buyer = get_input("Покупатель", buyer);
    buyer_addr = get_input("Адрес покупателя", buyer_addr);
    buyer_inn_kpp = get_input("ИНН/КПП покупателя", buyer_inn_kpp);

    // Товары/услуги
    std::vector<Item> items;
    char add_more = 'y';
    while (add_more == 'y' || add_more == 'Y') {
        Item item;
        item.name = get_input("Наименование товара/услуги", "Услуга по разработке ПО");
        item.unit_code = get_input("Код единицы измерения", "796");  // шт.
        item.unit_name = get_input("Обозначение единицы", "шт");
        item.quantity = get_double("Количество", 1.0);
        item.price = get_double("Цена за единицу", 10000.0);
        item.total_no_tax = item.quantity * item.price;
        item.excise = get_input("Акциз", "без акциза");
        item.tax_rate = get_input("Ставка налога", "20%");
        double tax_rate_num = 20.0;  // парсим если нужно, но по умолчанию 20%
        item.tax_amount = item.total_no_tax * (tax_rate_num / 100.0);
        item.total_with_tax = item.total_no_tax + item.tax_amount;
        item.country_code = get_input("Код страны происхождения", "-");
        item.country_name = get_input("Наименование страны", "-");
        item.customs_decl = get_input("Номер таможенной декларации", "-");

        items.push_back(item);

        std::cout << "Добавить ещё товар/услугу? (y/n): ";
        std::cin >> add_more;
        std::cin.ignore();  // очистка буфера
    }

    generate_latex(number, date, seller, seller_addr, seller_inn_kpp, shipper, consignee, payment_doc, buyer, buyer_addr, buyer_inn_kpp, items);

    return 0;
}
