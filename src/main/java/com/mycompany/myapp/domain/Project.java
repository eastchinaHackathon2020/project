package com.mycompany.myapp.domain;

import javax.persistence.*;
import javax.validation.constraints.NotNull;
import java.io.Serializable;
import java.time.Instant;
import java.util.HashMap;
import java.util.Map;

@Entity
@Table(name = "jhi_persistent_audit_event")
public class Project implements Serializable {

    private static final long serialVersionUID = 1L;

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "id")
    private Long id;

    @NotNull
    @Column(name = "repo_url", nullable = false)
    private String repoUrl;

    @NotNull
    @Column(name = "sha", nullable = false)
    private String sha;

    @NotNull
    @Column(name = "image_url", nullable = false)
    private String imageUrl;

    @NotNull
    @Column(name = "api_url", nullable = false)
    private String apiUrl;

    @NotNull
    @Column(name = "target_milestone", nullable = false)
    private String targetMilestone;

    @NotNull
    @Column(name = "user_id", nullable = false)
    private Long userId;

    public static long getSerialVersionUID() {
        return serialVersionUID;
    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public String getRepoUrl() {
        return repoUrl;
    }

    public void setRepoUrl(String repoUrl) {
        this.repoUrl = repoUrl;
    }

    public String getSha() {
        return sha;
    }

    public void setSha(String sha) {
        this.sha = sha;
    }

    public String getImageUrl() {
        return imageUrl;
    }

    public void setImageUrl(String imageUrl) {
        this.imageUrl = imageUrl;
    }

    public String getTargetMilestone() {
        return targetMilestone;
    }

    public void setTargetMilestone(String targetMilestone) {
        this.targetMilestone = targetMilestone;
    }

    public Long getUserId() {
        return userId;
    }

    public void setUserId(Long userId) {
        this.userId = userId;
    }

    public String getApiUrl() {
        return apiUrl;
    }

    public void setApiUrl(String apiUrl) {
        this.apiUrl = apiUrl;
    }

    @Override
    public int hashCode() {
        return 31;
    }

    @Override
    public String toString() {
        return "Project{" +
            "id=" + id +
            ", repoUrl='" + repoUrl + '\'' +
            ", sha='" + sha + '\'' +
            ", imageUrl='" + imageUrl + '\'' +
            ", apiUrl='" + apiUrl + '\'' +
            ", targetMilestone='" + targetMilestone + '\'' +
            ", userId=" + userId +
            '}';
    }
}
