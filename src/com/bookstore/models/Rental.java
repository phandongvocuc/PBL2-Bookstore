package com.bookstore.models;

import java.io.Serializable;
import java.time.LocalDate;
import java.time.temporal.ChronoUnit;

public class Rental implements Serializable {
    private String rentalId;
    private String userId;
    private String bookIsbn;
    private LocalDate rentalDate;
    private LocalDate dueDate;
    private LocalDate returnDate;
    private double fees;
    private String status; // "ACTIVE", "RETURNED", "OVERDUE"
    private boolean extended;

    public Rental(String rentalId, String userId, String bookIsbn, LocalDate rentalDate, int loanDays) {
        this.rentalId = rentalId;
        this.userId = userId;
        this.bookIsbn = bookIsbn;
        this.rentalDate = rentalDate;
        this.dueDate = rentalDate.plusDays(loanDays);
        this.status = "ACTIVE";
        this.extended = false;
    }

    public boolean isOverdue() {
        return LocalDate.now().isAfter(dueDate) && "ACTIVE".equals(status);
    }
    
    public long getDaysOverdue() {
        if (!isOverdue()) return 0;
        return ChronoUnit.DAYS.between(dueDate, LocalDate.now());
    }
    
    public double calculateOverdueFees(double dailyRate) {
        return getDaysOverdue() * dailyRate;
    }
    
    public void extendDueDate(int additionalDays) {
        this.dueDate = this.dueDate.plusDays(additionalDays);
        this.extended = true;
    }
    
    public void markAsReturned() {
        this.returnDate = LocalDate.now();
        this.status = "RETURNED";
    }

    public String getRentalId() { return rentalId; }
    public String getUserId() { return userId; }
    public String getBookIsbn() { return bookIsbn; }
    public LocalDate getRentalDate() { return rentalDate; }
    public LocalDate getDueDate() { return dueDate; }
    public LocalDate getReturnDate() { return returnDate; }
    public double getFees() { return fees; }
    public void setFees(double fees) { this.fees = fees; }
    public String getStatus() { return status; }
    public boolean isExtended() { return extended; }

    @Override
    public String toString() {
        return String.format("Rental[%s] User:%s Book:%s Due:%s Status:%s", 
                rentalId, userId, bookIsbn, dueDate, status);
    }
}