
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxseat 5
#define stacksize 10
#define qsize 10

struct Seat {
    int seatnmb;
    char customer[50];
    struct Seat* next;
};

struct Action {
    char action[10];
    int seatnmb;
    char customer[50];
};

struct Action stack[stacksize];
int top = -1;

char waiting[qsize][50];
int front = -1, rear = -1;

struct Seat* bookedseat = NULL;

void push(char action[], int seatnmb, char customer[]) {
    top++;
    strcpy(stack[top].action, action);
    stack[top].seatnmb = seatnmb;
    strcpy(stack[top].customer, customer);
}

struct Action pop() {
    return stack[top--];
}

void enqueue(char name[]) {
    if (rear == qsize - 1)
        return;
    if (front == -1)
    front = 0;
    rear++;

    strcpy(waiting[rear], name);
}

char* dq() {
    if (front == -1 || front > rear)
        return NULL;
          return waiting[front++];
}

int seatcount(struct Seat* head) {
    int count = 0;
    while (head) {
        count++;
        head = head->next;
    }
    return count;
}

void bookTicket(char name[]) {
    if (seatcount(bookedseat) >= maxseat) {

        printf("Show full! Adding %s to waiting list.\n", name);
        enqueue(name);

    return;
    }


    struct Seat* newseat = (struct Seat*)malloc(sizeof(struct Seat));

    newseat->seatnmb = seatcount(bookedseat) + 1;

    strcpy(newseat->customer, name);

    newseat->next = bookedseat;

    bookedseat = newseat;

    push("BOOK", newseat->seatnmb, name);

    printf("Ticket booked for %s (Seat %d).\n", name, newseat->seatnmb);
}

void ticketcancel(int seatnmb) {
    struct Seat *temp = bookedseat,

     *prev = NULL;

    while (temp && temp->seatnmb != seatnmb) {
        prev = temp;

        temp = temp->next;
    }

    if (!temp) {
        printf("Seat not found!\n");
        return;
    }

    push("CANCEL", temp->seatnmb, temp->customer);

    if (prev) prev->next = temp->next;

    else bookedseat = temp->next;

    free(temp);
    printf("Seat %d cancelled.\n", seatnmb);

    char* nextcustomer = dq();
    if (nextcustomer) {

        printf("Allocating seat to waiting customer: %s\n", nextcustomer);
        bookTicket(nextcustomer);
    }
}

void viewBooked() {

    struct Seat* temp = bookedseat;
    if (!temp) {

        printf("No seats booked.\n");
        return;
    }
    printf("Booked Seats:\n");
    while (temp) {
        printf("Seat %d - %s\n", temp->seatnmb, temp->customer);
        temp = temp->next;
    }
}

void undo() {
    if (top == -1) {
        printf("No actions to undo.\n");
        return;
    }
    struct Action last = pop();
    if (strcmp(last.action, "BOOK") == 0) {
        ticketcancel(last.seatnmb);
    } else if (strcmp(last.action, "CANCEL") == 0) {
        bookTicket(last.customer);
    }
}

int main() {
    int choice, seatnmb;
    char name[50];

    while (1) {
        printf("\n=== CINEMA TICKET BOOKING ===\n");
        printf("1. Book Ticket\n2. Cancel Ticket\n3. View Booked Seats\n4. Undo Last Action\n5. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter name: ");
                scanf("%s", name);
                bookTicket(name);
                break;
            case 2:
                printf("Enter seat number to cancel: ");
                scanf("%d", &seatnmb);
                ticketcancel(seatnmb);
                break;
            case 3:
                viewBooked();
                break;
            case 4:
                undo();
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }
}
