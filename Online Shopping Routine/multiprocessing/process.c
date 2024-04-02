#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#define NUM_CUSTOMERS 3
#define NUM_PRODUCTS 5

typedef struct Product
{
    int product_ID;
    double product_Price;
    int product_Quantity;
} Product;

Product products[NUM_PRODUCTS];
pthread_mutex_t locks[NUM_PRODUCTS];
pthread_mutex_t lock_1;
pthread_mutex_t lock;

typedef struct Item
{
    int ID;
    int quantity;
} Item;

typedef struct Customer
{
    int customer_ID;
    double customer_Balance;
    int num_ordered_Items;
    int num_purchased_Items;
    Item ordered_Items[5];
    Item purchased_Items[5];
    pthread_attr_t attr;
} Customer;

int order_product(int product_id, int quantity)
{
    Product product = products[product_id - 1];
    int x = 0; // I want a variable for informing if the product is sold or not

    pthread_mutex_lock(&locks[product_id - 1]); // locking for preventing race condition
    if (product.product_Quantity < quantity)    // when there is not enough product for shopping
    {
        x = -1;
    }
    else
    {
        products[product_id - 1].product_Quantity -= quantity; // increasing the quantity
    }

    pthread_mutex_unlock(&locks[product_id - 1]);

    return x;
}

void *order_products(void *ptr)
{

    Customer *args = (Customer *)ptr;
    double init_balance = args->customer_Balance;

    // pthread_mutex_lock(&lock);

    for (int i = 0; i < args->num_ordered_Items; i++)
    {
        int product_id = args->ordered_Items[i].ID;
        Product product = products[product_id - 1];
        double price = args->ordered_Items[i].quantity * product.product_Price; // calculating the price

        if (args->customer_Balance >= price) // when customer has enough money
        {

            if (order_product(args->ordered_Items[i].ID, args->ordered_Items[i].quantity) != -1) // when customer bought the product
            {
                args->customer_Balance = args->customer_Balance - price;                   // decresing the balance
                args->purchased_Items[args->num_purchased_Items] = args->ordered_Items[i]; // holding purchased item
                args->num_purchased_Items++;
                // pthread_mutex_lock(&lock);
                printf("Customer %d:\n", args->customer_ID);
                printf("İnitial Products:\n");
                printf(" Product ID    Quantity    Price\n");
                for (int i = 0; i < NUM_PRODUCTS; i++)
                {
                    printf("%d             %d             %f \n", products[i].product_ID, products[i].product_Quantity,
                           products[i].product_Price);
                }
                printf("Bought %d of product %d for $%f \n", args->ordered_Items[i].quantity, product_id, price);
                printf("Updated Products:\n");
                printf(" Product ID    Quantity    Price\n");
                for (int x = 0; x < NUM_PRODUCTS; x++)
                {
                    printf("%d          %d         %f \n", products[x].product_ID,
                           products[x].product_Quantity, products[x].product_Price);
                }
                printf("Customer%d (%d,%d) success! Paid $%f for each\n", args->customer_ID, args->ordered_Items[i].ID,
                       args->ordered_Items[i].quantity, product.product_Price);
                // pthread_mutex_unlock(&lock);
            }
            else // althogh customer is enough money there in not enough product left in stock (it means order_product return -1)
            {
                printf("Customer%d (%d,%d) fail! Only %d left in stock.\n", args->customer_ID, args->ordered_Items[i].ID,
                       args->ordered_Items[i].quantity, product.product_Quantity);
            }
        }
        else // customer doesnt have enough money
        {
            printf("Customer%d (%d,%d) fail! Insufficient funds.\n", args->customer_ID, args->ordered_Items[i].ID,
                   args->ordered_Items[i].quantity);
        }
    }
}

int main()
{

    srand(time(NULL));
    clock_t start, end; // for calculating time
    double time_used;
    start = clock();
    int shared_memory;
    pid_t pid;
    Customer *customer;

    shared_memory = shmget(IPC_PRIVATE, sizeof(customer), IPC_CREAT | 0666);
    if (shared_memory < 0)
    {
        printf("shmget error");
        exit(1);
    }
    customer = (Customer *)shmat(shared_memory, NULL, 0);
    if (customer == (Customer *)-1)
    {
        printf("shmget error");
        exit(1);
    }

    for (int i = 0; i < NUM_PRODUCTS; i++)
    {
        products[i].product_ID = i + 1;
        products[i].product_Price = rand() % 200 + 1;
        products[i].product_Quantity = rand() % 10 + 1;
    }

    double initial_balance[5];

    for (int i = 0; i < NUM_CUSTOMERS; i++)
    {
        customer[i].customer_ID = i + 1;
        customer[i].customer_Balance = rand() % 201 + 1000;
        initial_balance[i] = customer[i].customer_Balance;
        customer[i].num_ordered_Items = rand() % 5 + 1;
        customer[i].num_purchased_Items = 0;
        for (int j = 0; j < customer[i].num_ordered_Items; j++)
        {
            customer[i].ordered_Items[j].ID = rand() % NUM_PRODUCTS + 1;
            customer[i].ordered_Items[j].quantity = rand() % 10 + 1;
        }

        pthread_attr_init(&customer[i].attr);
    }

    for (int i = 0; i < NUM_CUSTOMERS; i++)
    {
        int xy = fork();
        if (xy < 0)
        {
            printf("Fork failed.");
            exit(1);
        }
        if (xy == 0)
        {

            order_products(&customer[i]);
            exit(0);
        }
    }
    for (int i = 0; i < NUM_CUSTOMERS; i++)
    {
        wait(NULL);
    }

    for (int i = 0; i < NUM_CUSTOMERS; i++)
    {
        printf("***Customer%d---------\n", customer[i].customer_ID);
        printf("İnitial balance %.2f\n", initial_balance[i]);
        printf("Updated balance %.2f\n", customer[i].customer_Balance);
        printf("Ordered products\n");
        printf("id    quantity\n");
        for (int j = 0; j < customer[i].num_ordered_Items; j++)
        {
            printf("%d       %d\n", customer[i].ordered_Items[j].ID, customer[i].ordered_Items[j].quantity);
        }
        printf(" \n");
        printf("Purchased products\n");
        printf("id    quantity\n");
        for (int j = 0; j < customer[i].num_purchased_Items; j++)
        {
            printf("%d       %d\n", customer[i].purchased_Items[j].ID, customer[i].purchased_Items[j].quantity);
        }
    }

    shmdt(customer);
    shmctl(shared_memory, IPC_RMID, NULL);
    end = clock();
    time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time: %f", time_used);
}
