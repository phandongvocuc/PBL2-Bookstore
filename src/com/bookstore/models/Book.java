package com.bookstore.models;

import java.io.Serializable;
import java.time.LocalDate;

public class Book implements Serializable {
    private String isbn;
    private String title;
    private String author;
    private String genre;
    private LocalDate publicationDate;
    private int quantity;
    private String condition;   // "Available", "Lost", "Damaged"
    private double price;

    public Book() {}

    public Book(String isbn, String title, String author, String genre,
                LocalDate publicationDate, int quantity, String condition, double price) {
        this.isbn = isbn;
        this.title = title;
        this.author = author;
        this.genre = genre;
        this.publicationDate = publicationDate;
        this.quantity = quantity;
        this.condition = condition;
        this.price = price;
    }

    public boolean isAvailable() {
        return quantity > 0 && "Available".equals(condition);
    }
    
    public void decreaseQuantity() {
        if (quantity > 0) {
            quantity--;
        }
    }
    
    public void increaseQuantity() {
        quantity++;
    }
    
    public void markAsLost() {
        this.condition = "Lost";
        this.quantity = 0; 
    }
    
    public void markAsAvailable() {
        this.condition = "Available";
    }
    
    public double calculateCompensation() {
        return this.price * 1.5; 
    }

    public String getIsbn() { return isbn; }
    public void setIsbn(String isbn) { this.isbn = isbn; }

    public String getTitle() { return title; }
    public void setTitle(String title) { this.title = title; }

    public String getAuthor() { return author; }
    public void setAuthor(String author) { this.author = author; }

    public String getGenre() { return genre; }
    public void setGenre(String genre) { this.genre = genre; }

    public LocalDate getPublicationDate() { return publicationDate; }
    public void setPublicationDate(LocalDate publicationDate) { this.publicationDate = publicationDate; }

    public int getQuantity() { return quantity; }
    public void setQuantity(int quantity) { this.quantity = quantity; }

    public String getCondition() { return condition; }
    public void setCondition(String condition) { this.condition = condition; }

    public double getPrice() { return price; }
    public void setPrice(double price) { this.price = price; }

    @Override
    public String toString() {
        return String.format("[%s] %s by %s | %s | %d copies | %s | $%.2f | %s",
                isbn, title, author, genre, quantity, condition, price, 
                isAvailable() ? "AVAILABLE" : "UNAVAILABLE");
    }
}