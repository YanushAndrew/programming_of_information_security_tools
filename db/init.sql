CREATE TABLE IF NOT EXISTS users (
    uuid UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    login VARCHAR(255) NOT NULL,
    password VARCHAR(255) NOT NULL,
    address VARCHAR(255) NOT NULL
);

-- Mock Data
INSERT INTO users (login, password, address) VALUES
('john_doe', '#ITSE$n#$eITfzsKx|REvuLk', '123 Maple Street, Springfield'),
('jane_smith', 'M@cIjWoLJN2UL6F#4Ow7Jh0-', '456 Oak Avenue, Metropolis'),
('alice_jones', 'U*LVrl4pOC6w|YXdMziBGUMp', '789 Pine Lane, Gotham');
