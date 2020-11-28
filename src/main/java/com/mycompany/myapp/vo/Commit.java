package com.mycompany.myapp.vo;

public class Commit {
    private String date;
    private String author;
    private String message;

    public String getDate() {
        return date;
    }

    public void setDate(String date) {
        this.date = date;
    }

    public String getAuthor() {
        return author;
    }

    public void setAuthor(String author) {
        this.author = author;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    @Override
    public String toString() {
        return "Commit{" +
                "data='" + date + '\'' +
                ", author='" + author + '\'' +
                ", message='" + message + '\'' +
                '}';
    }
}
