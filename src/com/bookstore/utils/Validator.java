package com.bookstore.utils;

import java.time.LocalDate;
import java.time.format.DateTimeParseException;
import java.util.regex.Pattern;

public class Validator {
    
	// USER VALIDATION
    
    public static boolean isValidUserId(String userId) {
        if (userId == null) return false;
        // Format: R001 (Reader), S001 (Staff), A001 (Admin)
        return Pattern.matches("^[RSA]\\d{3}$", userId);
    }
    
    public static boolean isValidName(String name) {
        if (name == null || name.trim().isEmpty()) return false;
        // Names should contain only letters, spaces, and common punctuation
        return Pattern.matches("^[a-zA-ZÀ-ÿ\\s\\-.']{2,50}$", name.trim());
    }
    
    public static boolean isValidCCCD(String cccd) {
        if (cccd == null) return false;
        // Vietnamese CCCD: 12 digits
        return Pattern.matches("^\\d{12}$", cccd);
    }
    
    public static boolean isValidAge(int age) {
        return age >= 6 && age <= 120; // Reasonable age range for library users
    }
    
    public static boolean isValidVietnamesePhone(String phone) {
        if (phone == null) return false;
        // Vietnamese phone numbers: 10-11 digits starting with 0
        return Pattern.matches("^(0|\\+84)(3[2-9]|5[6|8|9]|7[0|6-9]|8[1-9]|9[0-9])[0-9]{7}$", phone);
    }
    
    public static boolean isValidAddress(String address) {
        if (address == null || address.trim().isEmpty()) return false;
        return address.trim().length() >= 5 && address.trim().length() <= 200;
    }
    
    public static boolean isValidRole(String role) {
        if (role == null) return false;
        return role.equals("READER") || role.equals("STAFF") || role.equals("ADMIN");
    }
    
    // BOOK VALIDATION
    
    public static boolean isValidISBN(String isbn) {
        if (isbn == null) return false;
        // ISBN-10 or ISBN-13 format
        return Pattern.matches("^(?:\\d{9}[\\dXx]|\\d{13})$", isbn.replaceAll("-", ""));
    }
    
    public static boolean isValidBookTitle(String title) {
        if (title == null || title.trim().isEmpty()) return false;
        return title.trim().length() >= 1 && title.trim().length() <= 200;
    }
    
    public static boolean isValidAuthor(String author) {
        if (author == null || author.trim().isEmpty()) return false;
        return Pattern.matches("^[a-zA-ZÀ-ÿ\\s\\-.']{2,100}$", author.trim());
    }
    
    public static boolean isValidGenre(String genre) {
        if (genre == null || genre.trim().isEmpty()) return false;
        return genre.trim().length() >= 2 && genre.trim().length() <= 50;
    }
    
    public static boolean isValidPublicationDate(LocalDate date) {
        if (date == null) return false;
        return !date.isAfter(LocalDate.now()) && date.isAfter(LocalDate.of(1000, 1, 1));
    }
    
    public static boolean isValidQuantity(int quantity) {
        return quantity >= 0 && quantity <= 1000; // Reasonable inventory limits
    }
    
    public static boolean isValidBookCondition(String condition) {
        if (condition == null) return false;
        return condition.equals("Available") || condition.equals("Lost") || 
               condition.equals("Damaged") || condition.equals("Maintenance");
    }
    
    public static boolean isValidPrice(double price) {
        return price >= 0.0 && price <= 10000.0; // Reasonable book price range
    }
    
    // RENTAL VALIDATION 
    
    public static boolean isValidRentalId(String rentalId) {
        if (rentalId == null) return false;
        // Format: RT001
        return Pattern.matches("^RT\\d{3}$", rentalId);
    }
    
    public static boolean isValidRentalPeriod(LocalDate startDate, LocalDate endDate) {
        if (startDate == null || endDate == null) return false;
        return !startDate.isAfter(endDate) && 
               !startDate.isBefore(LocalDate.now().minusDays(1)) && // Can't rent in the past
               !endDate.isAfter(LocalDate.now().plusYears(1)); // Max 1 year rental
    }
    
    public static boolean isValidRentalStatus(String status) {
        if (status == null) return false;
        return status.equals("ACTIVE") || status.equals("RETURNED") || 
               status.equals("OVERDUE") || status.equals("LOST");
    }
    
    // REPORT VALIDATION
    
    public static boolean isValidReportId(String reportId) {
        if (reportId == null) return false;
        // Format: RPT202401
        return Pattern.matches("^RPT\\d{6}$", reportId);
    }
    
    public static boolean isValidMonth(String month) {
        if (month == null) return false;
        String[] validMonths = {
            "January", "February", "March", "April", "May", "June",
            "July", "August", "September", "October", "November", "December"
        };
        for (String validMonth : validMonths) {
            if (validMonth.equalsIgnoreCase(month)) return true;
        }
        return false;
    }
    
    public static boolean isValidReportData(int booksBorrowed, int lateReturns, 
                                          double fines, double revenue) {
        return booksBorrowed >= 0 && lateReturns >= 0 && 
               lateReturns <= booksBorrowed &&
               fines >= 0.0 && revenue >= 0.0;
    }
    
    // SYSTEM CONFIG VALIDATION 
    
    public static boolean isValidMaxLoanDays(int days) {
        return days >= 1 && days <= 365; // Reasonable loan period
    }
    
    public static boolean isValidDailyFine(double fine) {
        return fine >= 0.0 && fine <= 100.0; // Reasonable fine amount
    }
    
    // STRING VALIDATION UTILITIES 
    
    public static boolean isNullOrEmpty(String str) {
        return str == null || str.trim().isEmpty();
    }
    
    public static String sanitizeInput(String input) {
        if (input == null) return "";
        // Remove extra whitespace and potentially dangerous characters
        return input.trim().replaceAll("[<>\"']", "");
    }
    
    public static boolean isValidEmail(String email) {
        if (email == null) return false;
        // Basic email validation
        return Pattern.matches("^[A-Za-z0-9+_.-]+@(.+)$", email);
    }
    
    // DATE VALIDATION 
    
    public static boolean isFutureDate(LocalDate date) {
        return date != null && date.isAfter(LocalDate.now());
    }
    
    public static boolean isPastDate(LocalDate date) {
        return date != null && date.isBefore(LocalDate.now());
    }
    
    public static LocalDate parseDate(String dateString) {
        try {
            return LocalDate.parse(dateString);
        } catch (DateTimeParseException e) {
            return null;
        }
    }
    
    // VALIDATION RESULT CLASS 
    
    public static class ValidationResult {
        private final boolean valid;
        private final String message;
        
        public ValidationResult(boolean valid, String message) {
            this.valid = valid;
            this.message = message;
        }
        
        public boolean isValid() { return valid; }
        public String getMessage() { return message; }
        
        public static ValidationResult valid() {
            return new ValidationResult(true, "Valid");
        }
        
        public static ValidationResult invalid(String message) {
            return new ValidationResult(false, message);
        }
    }
    
    // COMPREHENSIVE VALIDATION METHODS 
    
    public static ValidationResult validateUserData(String id, String name, String cccd, 
                                                   int age, String phone, String address) {
        if (!isValidUserId(id)) {
            return ValidationResult.invalid("Invalid user ID format. Expected: R001, S001, A001");
        }
        if (!isValidName(name)) {
            return ValidationResult.invalid("Invalid name. Must be 2-50 characters, letters only");
        }
        if (!isValidCCCD(cccd)) {
            return ValidationResult.invalid("Invalid CCCD. Must be 12 digits");
        }
        if (!isValidAge(age)) {
            return ValidationResult.invalid("Invalid age. Must be between 6 and 120");
        }
        if (!isValidVietnamesePhone(phone)) {
            return ValidationResult.invalid("Invalid Vietnamese phone number");
        }
        if (!isValidAddress(address)) {
            return ValidationResult.invalid("Invalid address. Must be 5-200 characters");
        }
        
        return ValidationResult.valid();
    }
    
    public static ValidationResult validateBookData(String isbn, String title, String author, 
                                                   int quantity, double price) {
        if (!isValidISBN(isbn)) {
            return ValidationResult.invalid("Invalid ISBN format");
        }
        if (!isValidBookTitle(title)) {
            return ValidationResult.invalid("Invalid book title. Must be 1-200 characters");
        }
        if (!isValidAuthor(author)) {
            return ValidationResult.invalid("Invalid author name");
        }
        if (!isValidQuantity(quantity)) {
            return ValidationResult.invalid("Invalid quantity. Must be 0-1000");
        }
        if (!isValidPrice(price)) {
            return ValidationResult.invalid("Invalid price. Must be between 0 and 10,000");
        }
        
        return ValidationResult.valid();
    }
}