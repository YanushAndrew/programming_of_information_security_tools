# C++ Security Authenticator

This project is a C++ application that demonstrates a secure authentication system. It includes features like database integration, MAC address-based security, and mechanisms to prevent brute-force attacks.

## Disclaimer

This project was made for study purposes only. I am aware that hardcoding passwords and other sensitive information is not a secure practice in a real-world application. :3

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

You will need to have Git, a C++ compiler, and the PostgreSQL client libraries installed. On a Debian-based system (like Ubuntu or Kali), you can install them with:

```bash
sudo apt-get update
sudo apt-get install git g++ libpq-dev libpqxx-dev
```

You also need a running PostgreSQL server.

### Database Setup

1.  Connect to your PostgreSQL server.
2.  Create a database named `mydatabase`.
3.  Create a user named `user` with the password `password`.
4.  Grant the user privileges on the `mydatabase` database.
5.  Run the `db/init.sql` script to create the `users` table and populate it with initial data.

## Docker Setup

If you prefer using Docker, you can use the provided Docker Compose configuration to set up the PostgreSQL database.

1.  Make sure you have Docker and Docker Compose installed.
2.  Navigate to the `db` directory:
    ```bash
    cd prog_of_inf_indep/db
    ```
3.  Run Docker Compose:
    ```bash
    docker-compose up -d
    ```
This will start a PostgreSQL container with the database and user pre-configured.

## Compiling

Navigate to the `cpp` directory and run the following command to compile the application:

```bash
g++ -std=c++17 -o start start.cpp db.cpp file.cpp authenticator.cpp utils.cpp -lpqxx -lpq
```

After successful compilation, you can run the application with:

```bash
./start
```
