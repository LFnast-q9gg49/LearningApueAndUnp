#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <regex>

#include <libxml2/libxml/tree.h>

static const char *fmt_title = "raw data ( %d )";
static int is_change = 0;

int fetch_num(const xmlChar *str) {
    std::string input_string = reinterpret_cast<const char *>(str);
    std::regex re(R"(\(\s*(\d+)\s*\))");

    std::smatch match;
    std::regex_search(input_string, match, re);
    std::string match_str = match[1].str();
    int number = std::stoi(match_str);

    return number;
}

void operate_element_names(xmlNode *node) {
    xmlNode *cur_node;
    for (cur_node = node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {

            if (!xmlStrcmp(cur_node->name, (const xmlChar *) "graph")) {
                xmlSetProp(cur_node, (const xmlChar *) "autorange", (const xmlChar *) "z");
            }

            if (!xmlStrcmp(cur_node->name, (const xmlChar *) "legend")) {
                xmlSetProp(cur_node, (const xmlChar *) "loc", (const xmlChar *) "75% 15%");
            }

            if (!xmlStrcmp(cur_node->name, (const xmlChar *) "scatter")) {
                char *title_str = (char *)malloc(strlen(fmt_title));

                sprintf(title_str, fmt_title, fetch_num(xmlGetProp(cur_node, reinterpret_cast<const xmlChar *>("title"))));
                xmlSetProp(cur_node, (const xmlChar *) "title", (const xmlChar *)title_str);
            }

            if (!xmlStrcmp(cur_node->name, (const xmlChar *) "trace")) {
                xmlSetProp(cur_node, (const xmlChar *) "title", (const xmlChar *) "fitting");
            }

            if (!xmlStrcmp(cur_node->name, (const xmlChar *) "label")) {
                if (is_change){
                    break;
                }
                xmlSetProp(cur_node, (const xmlChar *) "loc", (const xmlChar *) "0.168u 0.437u");
                is_change = 1;
            }
        }
        operate_element_names(cur_node->children);
    }
}


int main(int argc, char **argv) {

    if (argc < 3) {
        fprintf(stderr, "Usage : ./main <src_file_path> <dst_file_path>\n");
        exit(1);
    }

    xmlDoc *doc_p ;
    xmlNode *root_p;

    doc_p = xmlReadFile(argv[1], nullptr, 0);
    if (doc_p == nullptr) {
        fprintf(stderr, "xml open error");
        exit(1);
    }

    root_p = xmlDocGetRootElement(doc_p);

    operate_element_names(root_p);

    xmlSaveFormatFileEnc(argv[2], doc_p, "UTF-8", 1);

    xmlFreeDoc(doc_p);
    xmlCleanupParser();

    return 0;
}

