#include <stdio.h>
#include <malloc.h>

const int BLOCKSIZE = 1;
int maxInput = (5 * BLOCKSIZE);

struct node {
    int i;
    struct node *next;
    struct node *prev;
};

struct list {
    struct node *head;  // left-most node in list
    struct node *tail;  // right-most node in list
    struct node *right; // last-added even integer
    struct node *left;  // last-added odd integer
    int length;   // number of nodes in free space
};

void printList(struct list *pList, int *counter) {
    struct node *cur = pList -> head;
    while (cur) {
        printf(" | %d | ", cur -> i);
        cur = cur -> next;
    }
    if (!(pList -> head) || !(pList -> tail) || *counter == 0) {
        printf(" | | ");
    }
    printf("\ncounter = %d\n", *counter);
}

void *allocator(int size, int *counter) {
    void *cur = malloc(size);
    if (cur) {
        (*counter)++;
    }
    return cur;
}

void deAllocator(void *p, int *counter) {
    if (p) {
        free(p);
        (*counter)--;
    }
}

void initialiseList(struct list *pList, int *counter) {
    pList -> head = allocator(sizeof(struct node), counter);
    pList -> tail = allocator(sizeof(struct node), counter);
    pList -> head -> next = pList -> tail;
    pList -> tail -> prev = pList -> head;
    pList -> head -> prev = NULL;
    pList -> tail -> next = NULL;
    pList -> head -> i = 0;
    pList -> tail -> i = 0;
    pList -> left = pList -> head;
    pList -> right = pList -> tail;
    pList -> length = 0;
}

void deAllocateBlock(struct list *pList, int *counter, int nNodes) {
    int n = (pList -> length) - nNodes;
    while ((pList -> length) > n) {
        struct node *temp = pList -> left -> next;
        pList -> left -> next -> next -> prev = pList -> left;
        pList -> left -> next = temp -> next;
        deAllocator(temp, counter);
        (pList->length)--;
    }
}

void allocateBlock(struct list *pList, int *counter, int nNodes) {
    int n = (pList -> length) + nNodes;
    while ((pList -> length) < n) {
        struct node *newNode = allocator(sizeof(struct node), counter);
        newNode -> prev = pList -> right -> prev;
        newNode -> next = pList -> right;
        pList -> right -> prev -> next = newNode;
        pList -> right -> prev = newNode;
        newNode -> i = -1;
        (pList -> length++);
    }
}

void freeList(struct list *pList, int *counter) {
    if ((pList -> left == pList -> head) && (pList -> right == pList -> tail) && (pList -> length == 0)) {
        deAllocator(pList -> head, counter);
        deAllocator(pList -> tail, counter);
        if (!(*counter)) {
            pList -> head = NULL;
            pList -> left = NULL;
            pList -> tail = NULL;
            pList -> right = NULL;
            pList -> length = -1;
        }
    }
}

void pushInt(struct list *pList, int *counter, int i) {
    if (pList -> length == 0) {
        allocateBlock(pList, counter, BLOCKSIZE);
    }
    if (i % 2 == 0) {
        pList -> right -> prev -> i = i;
        pList -> right = pList -> right -> prev;
    } else {
        pList -> left -> next -> i = i;
        pList -> left = pList -> left -> next;
    }
    (pList->length)--;
}

void pullInt(struct list *pList, int *counter, int i) {
    if (i % 2 == 0) {
        if (pList -> right != pList -> tail) {
            pList -> right -> i = -1;
            pList -> right = pList -> right -> next;
            (pList -> length)++;
        }
    } else {
        if (pList -> left != pList -> head) {
            pList -> left -> i = -1;
            pList -> left = pList -> left -> prev;
            (pList -> length)++;
        }
    }
    if ((pList -> length) > BLOCKSIZE) {
        deAllocateBlock(pList, counter, BLOCKSIZE);
    }
}

void clearList(struct list *pList, int *counter) {
    while ((pList -> head) != (pList -> left)) {
        pullInt(pList, counter, 1);
    }
    while ((pList -> tail) != (pList -> right)) {
        pullInt(pList, counter, 0);
    }
    deAllocateBlock(pList, counter, (pList -> length));
    freeList(pList, counter);
}

int getInput(char *s) {
    char c = '\0';
    int i = 0;
    while (c != '\n' && i < maxInput) {
        c = (char) getchar();
        if (c >= '0' && c <= '9') {
            s[i] = c;
            i++;
        }
    }
    if (i == maxInput) {
        while (c != '\n') {
            c = (char) getchar();
        }
    }
    return i;
}

int main() {
    int counter = 0;
    struct list myList;
    initialiseList(&myList, &counter);
    printList(&myList, &counter);
    char sPush[maxInput];
    char sPull[maxInput];
    int lenPush = getInput(sPush);
    int lenPull = getInput(sPull);
    int i = 0;
    int j = 0;
    while ((i + j) < (lenPush + lenPull)) {
        if (i < lenPush) {
            pushInt(&myList, &counter, (sPush[i] - '0'));
            i++;
            printList(&myList, &counter);
        }
        if (j < lenPull) {
            pullInt(&myList, &counter, (sPull[j] - '0'));
            j++;
            printList(&myList, &counter);
        }
    }
    printList(&myList, &counter);
    clearList(&myList, &counter);
    printList(&myList, &counter);
}