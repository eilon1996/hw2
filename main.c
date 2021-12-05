#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct buyInfo
{
    char chem_name[41];
    char chem_code[21];
    long quantity;
    long cost;
} buyInfo;

typedef struct invInfo
{
    char chem_name[41];
    char chem_code[21];
    long quantity;
} invInfo;

typedef struct chemNode
{
    invInfo data;
    struct chemNode *left;
    struct chemNode *right;
} chemNode;

typedef struct chemTree
{
    double balance;
    chemNode *root;
} chemTree;

/*
 ***file types***

1) Inventory files. For each chemical:
    Chemical name: Up to 40 characters.
    Chemical code: Up to 20 characters.
    Quantity in storage: Up to 9 digits.

    Examples:
    Ammonia NH3 1500
    Dihydrogen-Monoxide v=yi3erdgVVTw 600000

2) Sales files. Every sale file contains the following fields:
    Chemical code: Up to 20 characters.
    Quantity in storage: Up to 9 digits.
    Selling price: Up to 9 digits.

    Examples:
    C3H8 55000 70000
    NH3 1500 1200

3) Error files to record failed transactions:
    Chemical code: Up to 20 characters.
    Name of sales representative responsible: Up to 20 characters
    Quantity in failed sale: Up to 9 digits.

    Examples:
    C3H8 Jack_Black 200000000
    NH3 John_Green 1000
 */

//P1
void initInventory(char *invFileName, double bal, chemTree *invTree);
FILE *initErr(char *errFileName, FILE *currErr);

//P2
int saveInventory(char *invFileName, char *errFileName, chemTree *currInv, FILE *currErr);

//P3
void makeSale(char *saleFileName, chemTree *currInv, FILE *currErr);
/*
 The function receives as input aguments:
saleFileName : The name of a file containing a sale by one of the company's sales representatives.
currInv: A pointer to the tree containing the company's current inventory status and balance.
currErr: A file pointer to an open file (opened in append mode) containing reports of errors.

The function should do the following:
Open the file of name in saleFileName for reading and write the updated inventory after the sale into currInv.
For each item that is in the inventory in sufficient amount,
the function will update the amount of the item that remains in the currInv tree,
deleting its node if necessary. When the function terminates,
the balance should be updated (currInv bal) according to the sales carried out.
For each item contained in the sale that is either not in inventory is in inventory in a quantity
not sufficient to satisfy the sale (meaning, there isn't enough of it),
the sale of that item (in that line alone) is not carried out and an appropriate error in sale line is written to the error file.
Writing into the error file is to be done in lexicographic order sorted by chemical code.
 */

//P4
void makePurchase(struct buyInfo *buy, chemTree *currInv);
/*
 The function receives as input arguments:
buy : The details of a purchase done by the company.
currInv: A pointer to the tree containing the company's current inventory status and balance.

The function should do the following:
Search for the chemical in buy in the inventory tree currInv.
If found add the amount purchased to the quantity field in the appropriate node in the tree.
If not found insert a new node into the search tree for the chemical purchased, and fill its fields according to the information in buyFileName. The tree must remain a search tree sorted by chemical code
In either case, the balance should be updated (through currInv->balance) according to the purchase carried out (i.e. the cost of the purchased should be subtracted from the balance).

 */




void test2();
void invInfoToFile(invInfo data, FILE *invFile);
void treeToFile(chemNode *invTree, FILE *invFile);
chemTree * getExampleTree();
chemNode * getExampleTreeNode();
chemNode * createNode(invInfo data);


void test3();
chemNode * findInTree(chemNode *head, char * code);


int main() {
    printf("Hello, World!\n");
    test3();
    return 0;
}

//P3

void test3() {
    printf("*** test3 ***\n");

    printf("      -> findInTree:\n");
    chemTree *invTree = getExampleTree();
    if(strcmp(findInTree(invTree->root, "chem_code1")->data.chem_code, "chem_code1")) printf("              -> error line 1");
    if(strcmp(findInTree(invTree->root, "chem_code6")->data.chem_code, "chem_code6")) printf("              -> error line 2");
    if(strcmp(findInTree(invTree->root, "chem_code8")->data.chem_code, "chem_code8")) printf("              -> error line 3");
    if(findInTree(invTree->root, "chem_code10") != NULL) printf("              -> error line 4");
    if(findInTree(invTree->root, "chem_codd6") != NULL) printf("              -> error line 5");
    printf("         done\n");


    printf("      -> makeSale:\n");
    chemTree *currInv = getExampleTree();
    char *currErrFileName = "test3-makeSale-currErr";
    FILE * currErr = fopen(currErrFileName , "w");
    char * err = "C3H8 name1 2000\n"
                 "NH3 name2 1000\n";
    fputs(err, currErr);
    fclose(currErr);
    currErr = fopen(currErrFileName , "a");

    char *saleFileName = "test3-makeSale-sale";
    FILE * saleFile = fopen(saleFileName , "w");
    char * sale = "C3H8 2000 2000\n"       // not existing codes
                  "NH3 1000 1000\n"
                  "chem_code1 1000 1000\n" // too much qun'
                  "chem_code5 1000 1000\n"
                  "chem_code2 200 1000\n" // good examples
                  "chem_code6 100 1000\n";
    fputs(sale, saleFile);
    fclose(saleFile);

    makeSale(saleFileName, currInv, currErr);
    fclose(currErr);
    if(findInTree(currInv->root, "chem_code2")->data.quantity != 0)  printf("              -> error update tree1");
    if(findInTree(currInv->root, "chem_code6")->data.quantity != 600-100)  printf("              -> error update tree2");

    printf("         done\n");
}

chemNode * findInTree(chemNode *head, char * code){
    if(head == NULL)
        return NULL;
    if(strcmp(head->data.chem_code, code) == 0)
        return head;
    if(strcmp(head->data.chem_code, code) > 0)
        return findInTree(head -> left, code);
    else
        return findInTree(head -> right, code);
}


void makeSale(char *saleFileName, chemTree *currInv, FILE *currErr){
    FILE * saleFile = fopen(saleFileName , "r+");
    char chemCode[21], garbage[41];
    long quantity, price;
    chemNode * invHead = currInv->root;
    chemNode * wantedNode;

    while (fscanf(saleFile , "%s" , chemCode) != EOF) {  // remove name
        fscanf(saleFile, " %ld", &quantity);
        fscanf(saleFile, " %ld", &price);

        wantedNode = findInTree(invHead, chemCode);
        if(wantedNode == NULL || wantedNode->data.quantity < quantity){
            // add err
            fputs(chemCode, currErr);
            fputs(" ", currErr);
            fputs("name", currErr);     // ??? where do I get the name from ???
            fprintf(currErr, " %d\n", quantity);
        } else {
            wantedNode->data.quantity -= quantity;
            currInv->balance += price*quantity;
        }

    }

    fclose(saleFile);
}

//P2

void test2(){
    printf("*** test2 ***\n");

    printf("      -> treeToFile: ");
    chemNode * invTree = getExampleTreeNode();
    FILE * invFile = fopen("test2-treeToFile-invFileName" , "a+");
    treeToFile(invTree, invFile);
    fclose(invFile);
    printf("success\n");

    printf("      -> saveInventory: ");
    chemTree * currInv = getExampleTree();
    char *invFileName = "test2-saveInventory-invFileName";
    char *errFileName = "test2-saveInventory-errFileName";
    char *currErrFileName = "test2-saveInventory-currErr";
    FILE * currErr = fopen(currErrFileName , "w");
    char * err = "C3H8 Jack_Black 200000000\n"
                 "NH3 John_Green 1000\n";
    fputs(err, currErr);
    fclose(currErr);
    currErr = fopen(currErrFileName , "r");
    saveInventory(invFileName, errFileName, currInv, currErr);
    fclose(currErr);
    printf("success\n");
}

chemNode * createNode(invInfo data){
    chemNode * node = malloc(sizeof(chemNode));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

chemTree * getExampleTree(){
    chemTree * tree = malloc(sizeof(chemTree));
    tree->root = getExampleTreeNode();
    tree->balance = 123.321;
    return tree;
}

chemNode * getExampleTreeNode(){
    invInfo d1 = {"chem_name1", "chem_code1", 100};
    invInfo d2 = {"chem_name2", "chem_code2", 200};
    invInfo d3 = {"chem_name3", "chem_code3", 300};
    invInfo d4 = {"chem_name4", "chem_code4", 400};
    invInfo d5 = {"chem_name5", "chem_code5", 500};
    invInfo d6 = {"chem_name6", "chem_code6", 600};
    invInfo d7 = {"chem_name7", "chem_code7", 700};
    invInfo d8 = {"chem_name8", "chem_code8", 800};
    invInfo d9 = {"chem_name9", "chem_code9", 900};


    chemNode *head = createNode(d6);
    head->left = createNode(d4);
    head->right = createNode(d8);
    head->left->left = createNode(d2);
    head->left->right = createNode(d5);
    head->right->left = createNode(d7);
    head->right->right = createNode(d9);
    head->left->left->left = createNode(d1);
    head->left->left->right = createNode(d3);

    return head;

}

void invInfoToFile(invInfo data, FILE *invFile){
    fputs(data.chem_name, invFile);
    fputc(' ', invFile);
    fputs(data.chem_code, invFile);
    fputc(' ', invFile);
    fprintf(invFile, "%ld", data.quantity);
    fputc('\n', invFile);
}

void treeToFile(chemNode *invTree, FILE *invFile){
    // recursive function to write tree to open file    -preorder
    if (invTree != NULL) {
        invInfoToFile(invTree->data, invFile);
        treeToFile(invTree->left, invFile);
        treeToFile(invTree->right, invFile);
    }
}

int saveInventory(char *invFileName, char *errFileName, chemTree *currInv, FILE *currErr){
    // open invFileName and errFileName
    FILE * invFile = fopen(invFileName , "a+");
    FILE * errFile = fopen(errFileName , "a+");
    if (invFile == NULL) {
        fprintf(stderr, "Can't open invFile fil %s!\n", invFileName);
        exit(1);
    }
    if (errFile == NULL) {
        fprintf(stderr, "Can't open errFile fil %s!\n", errFileName);
        exit(1);
    }

    // currInv -> (a)invFileName
    treeToFile(currInv->root, invFile);
    // currErr -> (a)errFileName
    char c;
    while (EOF != (c=fgetc(currErr))) {
        fputc(c, errFile);
    }

    // close
    fclose(invFile);
    fclose(errFile);
}

