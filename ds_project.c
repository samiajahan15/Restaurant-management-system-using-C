#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// MENU (BST)-----

struct menuNode
{
    int code;      // product code 
    char name[50]; // product name
    double price;  // price in Tk 
    struct menuNode *left;
    struct menuNode *right;
};

struct menuNode *menuRoot = NULL;// global pointer to the root of the tree

struct menuNode *createMenuNode(int code, char n[], double p)
{
    struct menuNode *newnode;
    newnode = (struct menuNode *)malloc(sizeof(struct menuNode));//allocates memory for menu node
    newnode->code = code;
    strcpy(newnode->name, n);//strcpy(destination, source)
    newnode->price = p;
    newnode->left = NULL;
    newnode->right = NULL;
    return newnode;
}

struct menuNode *insertMenu(struct menuNode *root, int code, char n[], double p)
{
    if (root == NULL)
        return createMenuNode(code, n, p);

    if (code > root->code)
        root->right = insertMenu(root->right, code, n, p);
    else if (code < root->code)
        root->left = insertMenu(root->left, code, n, p);

    return root;
}

//to look up for menu details by code
struct menuNode *searchMenu(struct menuNode *root, int code)
{
    if (root == NULL)
        return NULL;
    if (code == root->code)
        return root;
    else if (code < root->code)
        return searchMenu(root->left, code);
    else
        return searchMenu(root->right, code);
}

// printing menu in order traversal 
void printMenu(struct menuNode *root)
{
    if (root == NULL)
        return;

    printMenu(root->left);
    printf("Code: %d | Name: %s | Price: %.2lf Tk\n",
           root->code, root->name, root->price);
    printMenu(root->right);
}

//deletion code when node has two children
struct menuNode *findMinMenu(struct menuNode *root)
{
    while (root != NULL && root->left != NULL)
        root = root->left;
    return root;
}

struct menuNode *deleteMenu(struct menuNode *root, int code)
{
    if (root == NULL)
        return root;

    if (code > root->code)
        root->right = deleteMenu(root->right, code);
    else if (code < root->code)
        root->left = deleteMenu(root->left, code);
    else
    {
        if (root->left == NULL)
        {
            struct menuNode *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            struct menuNode *temp = root->left;
            free(root);
            return temp;
        }
        else
        {
            struct menuNode *temp = findMinMenu(root->right);
            root->code = temp->code;
            strcpy(root->name, temp->name);
            root->price = temp->price;
            root->right = deleteMenu(root->right, temp->code);
        }
    }
    return root;
}

void addMenuItem()
{
    int code;
    char name[50];
    double price;

    printf("Enter new Food Code: ");
    scanf("%d", &code);
    printf("Enter Food Name (use_underscore_instead_of_space): ");
    scanf("%s", name);
    printf("Enter Food Price (Tk): ");
    scanf("%lf", &price);

    menuRoot = insertMenu(menuRoot, code, name, price);
    printf("Menu item added successfully.\n");
}

void deleteMenuItem()
{
    int code;
    struct menuNode *m;

    printf("Enter Food Code to delete: ");
    scanf("%d", &code);

    m = searchMenu(menuRoot, code);
    if (m == NULL)
    {
        printf("Food code %d not found.\n", code);
    }
    else
    {
        menuRoot = deleteMenu(menuRoot, code);
        printf("Food code %d deleted successfully.\n", code);
    }
}

/* ================= TABLES (ARRAY) ================= */

struct table
{
    int id;     /* table number */
    int status; /* 0 = FREE, 1 = OCCUPIED */
};

struct table tables[20];
int tableCount = 0;

void initTables(int n)
{
    int i;
    tableCount = n;
    for (i = 0; i < n; i++)
    {
        tables[i].id = i + 1;
        tables[i].status = 0; /* FREE */
    }
}

void showTables()
{
    int i;
    printf("\n--- Table Status ---\n");
    for (i = 0; i < tableCount; i++)
    {
        if (tables[i].status == 0)
            printf("Table %d : FREE\n", tables[i].id);
        else
            printf("Table %d : OCCUPIED\n", tables[i].id);
    }
}

int assignTable(int id)
{
    int i;
    for (i = 0; i < tableCount; i++)
    {
        if (tables[i].id == id)
        {
            if (tables[i].status == 0)
            {
                tables[i].status = 1;
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
    return 0;
}

void freeTable(int id)
{
    int i;
    for (i = 0; i < tableCount; i++)
    {
        if (tables[i].id == id)
        {
            tables[i].status = 0;
            return;
        }
    }
}

/* ================ ORDERS (LINKED LIST) ================= */

struct orderItem
{
    int menuCode;
    int quantity;
    struct orderItem *next;
};

struct order
{
    int orderId;
    int tableNo;
    int status; /* 0 NEW, 1 IN_KITCHEN, 2 READY, 3 SERVED */
    struct orderItem *itemStart;
    struct order *next;
};

struct order *orderStart = NULL;
int nextOrderId = 1;

struct order *createOrder(int tableNo)
{
    struct order *newOrder;
    newOrder = (struct order *)malloc(sizeof(struct order));

    newOrder->orderId = nextOrderId++;
    newOrder->tableNo = tableNo;
    newOrder->status = 0;
    newOrder->itemStart = NULL;

    newOrder->next = orderStart;
    orderStart = newOrder;

    return newOrder;
}

struct order *findOrderById(int id)
{
    struct order *i = orderStart;
    while (i != NULL && i->orderId != id)
        i = i->next;
    return i;
}

void addItemToOrder(struct order *ord, int menuCode, int quantity)
{
    struct orderItem *newItem;
    struct orderItem *i;

    newItem = (struct orderItem *)malloc(sizeof(struct orderItem));
    newItem->menuCode = menuCode;
    newItem->quantity = quantity;
    newItem->next = NULL;

    if (ord->itemStart == NULL)
    {
        ord->itemStart = newItem;
    }
    else
    {
        i = ord->itemStart;
        while (i->next != NULL)
            i = i->next;
        i->next = newItem;
    }
}

void removeItemFromOrder(struct order *ord, int menuCode)
{
    struct orderItem *curr = ord->itemStart;
    struct orderItem *prev = NULL;

    while (curr != NULL)
    {
        if (curr->menuCode == menuCode)
        {
            if (prev == NULL)
                ord->itemStart = curr->next;
            else
                prev->next = curr->next;
            free(curr);
            printf("Item with code %d removed from order.\n", menuCode);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("Item with code %d not found in order.\n", menuCode);
}

void showOrderDetails(struct order *ord)
{
    struct orderItem *i = ord->itemStart;
    struct menuNode *m;

    if (i == NULL)
    {
        printf("No items in this order.\n");
        return;
    }

    printf("\n--- Order Details ---\n");
    printf("Order ID: %d | Table: %d\n", ord->orderId, ord->tableNo);
    printf("Items:\n");

    while (i != NULL)
    {
        m = searchMenu(menuRoot, i->menuCode);
        if (m != NULL)
        {
            printf("Code: %d | Name: %s | Qty: %d | Subtotal: %.2lf Tk\n",
                   m->code, m->name, i->quantity, m->price * i->quantity);
        }
        i = i->next;
    }
}

double calculateBill(struct order *ord)
{
    double total = 0;
    struct orderItem *i = ord->itemStart;
    struct menuNode *m;

    while (i != NULL)
    {
        m = searchMenu(menuRoot, i->menuCode);
        if (m != NULL)
        {
            total += m->price * i->quantity;
        }
        i = i->next;
    }
    return total;
}

/* ================ QUEUES (KITCHEN & READY) ================= */

int kitchenQueue[100];
int kFront = 0, kRear = 0, kCount = 0;

int readyQueue[100];
int rFront = 0, rRear = 0, rCount = 0;

void enqueueKitchen(int orderId)
{
    if (kCount >= 100)
    {
        printf("Kitchen queue full!\n");
        return;
    }
    kitchenQueue[kRear] = orderId;
    kRear++;
    if (kRear == 100)
        kRear = 0;
    kCount++;
}

int dequeueKitchen()
{
    int id;
    if (kCount == 0)
        return -1;
    id = kitchenQueue[kFront];
    kFront++;
    if (kFront == 100)
        kFront = 0;
    kCount--;
    return id;
}

void enqueueReady(int orderId)
{
    if (rCount >= 100)
    {
        printf("Ready queue full!\n");
        return;
    }
    readyQueue[rRear] = orderId;
    rRear++;
    if (rRear == 100)
        rRear = 0;
    rCount++;
}

int dequeueReady()
{
    int id;
    if (rCount == 0)
        return -1;
    id = readyQueue[rFront];
    rFront++;
    if (rFront == 100)
        rFront = 0;
    rCount--;
    return id;
}

/* ================ HISTORY (LINKED LIST) ================= */

struct historyNode
{
    int orderId;
    int tableNo;
    double totalAmount;
    struct historyNode *next;
};

struct historyNode *historyStart = NULL;

void addToHistory(int orderId, int tableNo, double total)
{
    struct historyNode *newnode;
    newnode = (struct historyNode *)malloc(sizeof(struct historyNode));
    newnode->orderId = orderId;
    newnode->tableNo = tableNo;
    newnode->totalAmount = total;
    newnode->next = historyStart;
    historyStart = newnode;
}

void printHistory()
{
    struct historyNode *i = historyStart;
    if (i == NULL)
    {
        printf("No past orders in history.\n");
        return;
    }
    printf("\n=== Order History ===\n");
    while (i != NULL)
    {
        printf("Order %d | Table %d | Total: %.2lf Tk\n",
               i->orderId, i->tableNo, i->totalAmount);
        i = i->next;
    }
}

/* ================ PANELS ================= */

void cashierMenu()
{
    int choice;
    while (1)
    {
        printf("\n=== Cashier Panel ===\n");
        printf("1. Create New Order\n");
        printf("2. Add More Items to Existing Order\n");
        printf("3. Remove Item from Order\n");
        printf("4. Show Order Details\n");
        printf("5. Send Order to Kitchen\n");
        printf("6. Show Menu\n");
        printf("0. Back to Main Menu\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1)
            return;

        if (choice == 0)
            break;

        /* 1. Create order and take all items now */
        if (choice == 1)
        {
            int tableNo, ok;
            struct order *ord;

            printf("Enter table number: ");
            scanf("%d", &tableNo);
            ok = assignTable(tableNo);
            if (!ok)
            {
                printf("Table not available.\n");
            }
            else
            {
                int code, qty;
                struct menuNode *m;
                double subtotal, total;

                ord = createOrder(tableNo);
                printf("Created Order ID %d for Table %d\n",
                       ord->orderId, ord->tableNo);

                printf("\n--- Menu ---\n");
                printMenu(menuRoot);
                printf("Now taking items for this order.\n");
                printf("Enter Food Code = 0 when you are finished.\n");

                while (1)
                {
                    printf("\nEnter Food Code (0 to finish): ");
                    scanf("%d", &code);

                    if (code == 0)
                    {
                        total = calculateBill(ord);
                        printf("\nFinished taking order.\n");
                        printf("Order ID: %d | Table: %d | Total: %.2lf Tk\n",
                               ord->orderId, ord->tableNo, total);
                        break;
                    }

                    m = searchMenu(menuRoot, code);
                    if (m == NULL)
                    {
                        printf("Invalid code. Try again.\n");
                        continue;
                    }

                    printf("Enter quantity for %s: ", m->name);
                    scanf("%d", &qty);

                    addItemToOrder(ord, code, qty);

                    subtotal = m->price * qty;
                    total = calculateBill(ord);

                    printf("Added: Code: %d | Name: %s | Qty: %d | Subtotal: %.2lf Tk\n",
                           m->code, m->name, qty, subtotal);
                    printf("Current Order Total: %.2lf Tk\n", total);
                }
            }
        }
        /* 2. Add more items later (e.g. late friend) */
        else if (choice == 2)
        {
            int orderId;
            struct order *ord;

            printf("Enter existing Order ID: ");
            scanf("%d", &orderId);
            ord = findOrderById(orderId);
            if (ord == NULL)
            {
                printf("Order not found.\n");
                continue;
            }

            if (ord->status != 0)
            {
                printf("Cannot edit this order now (already sent to kitchen or served).\n");
                continue;
            }

            printf("\n--- Menu ---\n");
            printMenu(menuRoot);
            printf("Add more items to Order %d. Enter Food Code = 0 to finish.\n",
                   orderId);

            while (1)
            {
                int code, qty;
                struct menuNode *m;
                double subtotal, total;

                printf("\nEnter Food Code (0 to finish): ");
                scanf("%d", &code);

                if (code == 0)
                {
                    total = calculateBill(ord);
                    printf("Finished updating order. Current Total: %.2lf Tk\n", total);
                    break;
                }

                m = searchMenu(menuRoot, code);
                if (m == NULL)
                {
                    printf("Invalid code. Try again.\n");
                    continue;
                }

                printf("Enter quantity for %s: ", m->name);
                scanf("%d", &qty);

                addItemToOrder(ord, code, qty);

                subtotal = m->price * qty;
                total = calculateBill(ord);

                printf("Added: Code: %d | Name: %s | Qty: %d | Subtotal: %.2lf Tk\n",
                       m->code, m->name, qty, subtotal);
                printf("Current Order Total: %.2lf Tk\n", total);
            }
        }
        /* 3. Remove one item from an order */
        else if (choice == 3)
        {
            int orderId, code;
            struct order *ord;

            printf("Enter Order ID: ");
            scanf("%d", &orderId);
            ord = findOrderById(orderId);
            if (ord == NULL)
            {
                printf("Order not found.\n");
                continue;
            }

            if (ord->status != 0)
            {
                printf("Cannot edit this order now (already sent to kitchen or served).\n");
                continue;
            }

            printf("Enter Food Code to remove: ");
            scanf("%d", &code);
            removeItemFromOrder(ord, code);
        }
        /* 4. Show order details */
        else if (choice == 4)
        {
            int orderId;
            struct order *ord;
            double total;

            printf("Enter Order ID: ");
            scanf("%d", &orderId);
            ord = findOrderById(orderId);
            if (ord == NULL)
            {
                printf("Order not found.\n");
                continue;
            }
            showOrderDetails(ord);
            total = calculateBill(ord);
            printf("Current Total: %.2lf Tk\n", total);
        }
        /* 5. Send order to kitchen */
        else if (choice == 5)
        {
            int orderId;
            struct order *ord;

            printf("Enter Order ID to send to kitchen: ");
            scanf("%d", &orderId);
            ord = findOrderById(orderId);
            if (ord == NULL)
            {
                printf("Order not found.\n");
                continue;
            }

            if (ord->itemStart == NULL)
            {
                printf("Order has no items. Cannot send to kitchen.\n");
                continue;
            }

            enqueueKitchen(orderId);
            ord->status = 1; /* IN_KITCHEN */
            printf("Order %d sent to kitchen.\n", ord->orderId);
        }
        /* 6. Just show menu */
        else if (choice == 6)
        {
            printf("\n--- Menu ---\n");
            printMenu(menuRoot);
        }
        else
        {
            printf("Invalid choice.\n");
        }
    }
}

void chefMenu()
{
    int choice;
    while (1)
    {
        printf("\n=== Chef Panel ===\n");
        printf("1. Take Next Order from Kitchen\n");
        printf("0. Back to Main Menu\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1)
            return;

        if (choice == 0)
            break;

        if (choice == 1)
        {
            int orderId;
            struct order *ord;

            orderId = dequeueKitchen();
            if (orderId == -1)
            {
                printf("No orders in kitchen queue.\n");
            }
            else
            {
                ord = findOrderById(orderId);
                if (ord == NULL)
                {
                    printf("Order not found.\n");
                }
                else
                {
                    printf("Cooking Order %d for Table %d...\n",
                           ord->orderId, ord->tableNo);
                    showOrderDetails(ord);
                    ord->status = 2; /* READY */
                    enqueueReady(orderId);
                    printf("Order %d is READY.\n", ord->orderId);
                }
            }
        }
        else
        {
            printf("Invalid choice.\n");
        }
    }
}

void waiterMenu()
{
    int choice;
    while (1)
    {
        printf("\n=== Waiter Panel ===\n");
        printf("1. Serve Next Ready Order\n");
        printf("0. Back to Main Menu\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1)
            return;

        if (choice == 0)
            break;

        if (choice == 1)
        {
            int orderId;
            struct order *ord;
            double total;

            orderId = dequeueReady();
            if (orderId == -1)
            {
                printf("No ready orders.\n");
            }
            else
            {
                ord = findOrderById(orderId);
                if (ord == NULL)
                {
                    printf("Order not found.\n");
                }
                else
                {
                    printf("Serving Order %d for Table %d...\n", ord->orderId, ord->tableNo);
                    showOrderDetails(ord);
                    total = calculateBill(ord);
                    printf("Final Bill: %.2lf Tk\n", total);
                    addToHistory(ord->orderId, ord->tableNo, total);
                    freeTable(ord->tableNo);
                    ord->status = 3; /* SERVED */
                }
            }
        }
        else
        {
            printf("Invalid choice.\n");
        }
    }
}

void managerMenu()
{
    int choice;
    while (1)
    {
        printf("\n=== Manager Panel ===\n");
        printf("1. Show Tables\n");
        printf("2. Show Menu\n");
        printf("3. Add Menu Item\n");
        printf("4. Delete Menu Item\n");
        printf("5. Show Order History\n");
        printf("0. Back to Main Menu\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1)
            return;

        if (choice == 0)
            break;

        if (choice == 1)
        {
            showTables();
        }
        else if (choice == 2)
        {
            printf("\n--- Current Menu ---\n");
            printMenu(menuRoot);
        }
        else if (choice == 3)
        {
            addMenuItem();
        }
        else if (choice == 4)
        {
            deleteMenuItem();
        }
        else if (choice == 5)
        {
            printHistory();
        }
        else
        {
            printf("Invalid choice.\n");
        }
    }
}

/* ===================== MAIN ===================== */

int main()
{
    int choice;

    printf("Welcome to BFC Restaurant!\n");

    initTables(5); /* 5 tables */

    /* Default menu items */
    menuRoot = insertMenu(menuRoot, 1, "Spicy_Crispy_Fried_Chicken", 260);
    menuRoot = insertMenu(menuRoot, 2, "Regular_Crispy_Fried_Chicken", 260);
    menuRoot = insertMenu(menuRoot, 3, "Naga_Fried_Chicken", 270);
    menuRoot = insertMenu(menuRoot, 4, "Supreme_Cheese_Burger", 325);
    menuRoot = insertMenu(menuRoot, 5, "Best_Burger", 275);
    menuRoot = insertMenu(menuRoot, 6, "Juicy_Cheese_Burger", 225);
    menuRoot = insertMenu(menuRoot, 7, "French_Fries_Regular", 115);
    menuRoot = insertMenu(menuRoot, 8, "French_Fries_Large", 145);
    menuRoot = insertMenu(menuRoot, 9, "Coleslaw_Regular", 65);
    menuRoot = insertMenu(menuRoot, 10, "Soft_Drink", 40);

    while (1)
    {
        printf("\n=== Restaurant Management System ===\n");
        printf("1. Cashier Login\n");
        printf("2. Chef Login\n");
        printf("3. Waiter Login\n");
        printf("4. Manager Login\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1)
            break;

        if (choice == 0)
        {
            printf("Exiting system...\n");
            break;
        }
        else if (choice == 1)
        {
            cashierMenu();
        }
        else if (choice == 2)
        {
            chefMenu();
        }
        else if (choice == 3)
        {
            waiterMenu();
        }
        else if (choice == 4)
        {
            managerMenu();
        }
        else
        {
            printf("Invalid choice.\n");
        }
    }

    return 0;
}
