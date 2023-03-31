#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <libxml2/libxml/tree.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/xpath.h>
#include <libxml2/libxml/xmlversion.h>

void print_element_names(xmlNode *node) {
    xmlNode *cur_node = nullptr;
    for (cur_node = node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            std::cout << "Element name: " << cur_node->name << std::endl;
            xmlAttr *attr = cur_node->properties;
            while (attr) {
                std::cout << "\tAttribute name: " << attr->name << ", value: " << xmlGetProp(cur_node, attr->name) << std::endl;
                attr = attr->next;
            }
        }
        print_element_names(cur_node->children);
    }
}

int main(int argc, char **argv) {

    if (argc < 2){
        fprintf(stderr, "Usage : ./main <file_path>");
        exit(1);
    }

    xmlDoc *doc_p = nullptr;
    xmlNode *root_p = nullptr;

    doc_p = xmlReadFile(argv[1], nullptr, 0);
    if (doc_p == nullptr){
        fprintf(stderr, "xml open error");
        exit(1);
    }

    root_p = xmlDocGetRootElement(doc_p);

    print_element_names(root_p);

    xmlFreeDoc(doc_p);
    xmlCleanupParser();

    return 0;
}

