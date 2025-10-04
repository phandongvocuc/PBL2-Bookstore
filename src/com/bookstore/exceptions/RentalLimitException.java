package com.bookstore.exceptions;

public class RentalLimitException extends Exception {
    public RentalLimitException(String message) {
        super(message);
    }
}
