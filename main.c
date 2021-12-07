// Eilon Toledano 206903445
// Noam Akun 206784845

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define isInit if(invFileName[0] == '\0'){printf("inventory not initialized yet. Initialize before choosing any other option.\n"); continue;}

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




//P1
void initInventory(char *invFileName, double bal, chemTree *invTree);
FILE *initErr(char *errFileName, FILE *currErr);

// void buildBST(chemNode *root , invInfo *newChem);
invInfo * buildBST(chemNode *root , invInfo *newChem);
void destroyT(chemNode *root);


//P2
int saveInventory(char *invFileName, char *errFileName, chemTree *currInv, FILE *currErr);

void test2();
void invInfoToFile(invInfo data, FILE *invFile);
void treeToFile(chemNode *invTree, FILE *invFile);
chemTree * getExampleTree();
chemNode * getExampleTreeNode();
chemNode * createNode(invInfo data);


//P3
void makeSale(char *saleFileName, chemTree *currInv, FILE *currErr);

void test3();
chemNode * findInTree(chemNode *head, char * code);


//P4
void makePurchase(buyInfo *buy , chemTree *currInv);


//P Menu
void testMenu();
void printMenu(chemTree * cT);

void scani(int * d);
void scand(double * d);
void manegeMenu();


int main() {
    testMenu();
    return 0;
}

void testMenu(){

    printf("*** test menu ***\n");

    printf("      -> manegeMenu: \n");
    manegeMenu();
}

//MENU
void printMenu(chemTree * cT){
    double bal = 0;
    if(cT != NULL)
        bal = cT->balance;
    printf("Welcome to Chem-R-Us LTD database. Current company balance is %f.\n"
           "Please select an option from the following menu.\n"
           "1) Initialize company inventory and error report files.\n"
           "2) Save current Inventory and error status to files.\n"
           "3) Record a sale.\n"
           "4) Record a purchase.\n\n", bal);
}

void scans(char * str){
    fscanf(stdin, "%[^\n]%*c", str);
}
void scand(double * d){
    while(! fscanf(stdin, "%lf", d)) {
        printf("Error in floating point number input. Try again: ");
        fscanf(stdin, "%*[^\n]%*c");
    }
    while (getchar() != '\n');
}
void scani(int * d){
    while(! fscanf(stdin, "%d", d)) {
        printf("Error in floating point number input. Try again: ");
        fscanf(stdin, "%*[^\n]%*c");
    }
    while (getchar() != '\n');
}

void manegeMenu() {
    // Command not recognized. Try again.

    char input[201], invFileName[201] = {'\0'}, errFileName[201], saleFileName[201], c[201];
    double balance;
    FILE *invFile = NULL, *errFile = NULL;
    chemTree *invTree = malloc(sizeof(chemTree));
    invTree->root = NULL;
    invTree->balance = 0;
    buyInfo *buy = malloc(sizeof(buyInfo));
    while (1) {
        printMenu(invTree);

        scans(input);  // scans all the input and remove the \n in the end
        // c[0] = getchar();
        if (!strcmp(input, "0")) {
            return;
        } else if (!strcmp(input, "1")) {
            printf("Please enter inventory file name: ");
            scans(invFileName);
            printf("Please enter balance value: ");
            scand(&balance);


            printf("Please enter error file name: ");
            scans(errFileName);

            initInventory(invFileName, balance, invTree);
            errFile = initErr(errFileName, errFile);

            // TODO do the rest of the cases
        } else if (!strcmp(input, "2")) {
            isInit

            printf("Please enter inventory file name: ");
            scans(invFileName);
            printf("Please enter error file name: ");
            scans(errFileName);
            int res = saveInventory(invFileName, errFileName, invTree, errFile);

        } else if (!strcmp(input, "3")) {
            isInit

            printf("Please enter name of sale file:");
            scans(saleFileName);
            makeSale(saleFileName, invTree, errFile);

        } else if (!strcmp(input, "4")) {
            isInit
            // set buy
            printf("Please insert chemical name:");
            scans(buy->chem_name);
            printf("Please insert chemical code:");
            scans(buy->chem_code);
            printf("Please insert chemical quantity:");
            scani(&buy->quantity);
            printf("Please insert chemical cost:");
            scand(&buy->cost);

            makePurchase(buy , invTree);
        } else {
            printf("Command not recognized. Try again.\n");
        }
    }
}

//P1
void destroyT(chemNode *root){ //page 45 //postorder
    if (root != NULL) {
        destroyT(root -> left);
        destroyT(root -> right);
        free(root);
    }
}

void insert(chemNode ** root , invInfo *newChem)
{
    if (*root == NULL)
        *root = createNode(*newChem);
    else if (strcmp((*root) -> data .chem_code , newChem -> chem_code) > 0)
        insert(&(*root)->left, newChem);
    else
        insert(&(*root)->right, newChem);
}

invInfo * buildBST(chemNode *root , invInfo *newChem){

    if(root == NULL)
        return createNode(*newChem);
    if(strcmp(root -> data .chem_code , newChem -> chem_code) > 0)
        root -> left = buildBST(root -> left , newChem);
    else if(strcmp(root -> data .chem_code , newChem -> chem_code) < 0)
        root -> right = buildBST(root -> right , newChem);
    return root;
}

void initInventory(char *invFileName, double bal, chemTree *invTree){
    char nName [41] , nCode [21];
    long nQuantity;
    invInfo *newChem;

    // init of invTree
    destroyT(invTree->root);
    invTree->root = NULL;
    invTree -> balance = bal;

    FILE *invFile = fopen(invFileName, "r"); // Opens the file and sets a pointer named invFile
    if(invFile == NULL)
        printf("Error in openning inventory file. Inventory tree initialized as empty.\n");
    while (fscanf(invFile , "%s" , nName) != EOF){
        strcpy(newChem -> chem_name , nName);
        fscanf(invFile , "%s" , nCode);
        strcpy(newChem -> chem_code , nCode);
        fscanf(invFile , "%ld" , &nQuantity);
        newChem -> quantity = nQuantity;
        invTree -> root = buildBST(invTree -> root , newChem);
    }
    fclose(invFile);
}

FILE *initErr(char *errFileName, FILE *currErr){
    if(currErr != NULL)
        fclose(currErr);
    FILE *oFile = fopen(errFileName, "a");
    return oFile;
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
    if (invFile == NULL || errFile == NULL) {
        return 0;
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

    return 1;
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
    char chemCode[21];
    long quantity, price;
    chemNode * invHead = currInv->root;
    chemNode * wantedNode;

    char name[201];
    printf("Please enter name of sales representative (no spaces): ");
    scans(name);

    while (fscanf(saleFile , "%s" , chemCode) != EOF) {
        fscanf(saleFile, " %ld", &quantity);
        fscanf(saleFile, " %ld", &price);

        wantedNode = findInTree(invHead, chemCode);
        if(wantedNode == NULL || wantedNode->data.quantity < quantity){
            // add err
            fputs(chemCode, currErr);
            fputs(" ", currErr);
            fputs(name, currErr);     // TODO: check
            fprintf(currErr, " %d\n", quantity);
        } else {
            wantedNode->data.quantity -= quantity;
            currInv->balance += price;
        }

    }
    fclose(saleFile);
}


//P4
//TODO check function args from Maoz
//TODO make re-check
void makePurchase(buyInfo *buy , chemTree *currInv) {
    invInfo *newInfo = malloc(sizeof (invInfo));
    chemNode *invHead = currInv -> root;
    chemNode *wantedNode;
    wantedNode = findInTree(invHead , buy -> chem_code);
    if (wantedNode == NULL) {
        strcpy(newInfo -> chem_name , buy -> chem_name);
        strcpy(newInfo -> chem_code , buy -> chem_code);
        newInfo -> quantity = buy -> quantity;
        currInv -> root = buildBST(invHead , newInfo);
    }
    else
        wantedNode -> data.quantity += buy -> quantity;
    currInv -> balance -= buy -> cost;
    free(newInfo);
}
