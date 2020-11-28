package com.mycompany.myapp.vo;

import javax.persistence.Column;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.validation.constraints.NotNull;
import java.util.List;

public class ProjectVO {
    private Long id;

    private String repoUrl;

    private String sha;

    private String imageUrl;

    private String apiUrl;

    private String targetMilestone;

    private Long userId;

    private List<TaskVO> tasks;

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

    public String getApiUrl() {
        return apiUrl;
    }

    public void setApiUrl(String apiUrl) {
        this.apiUrl = apiUrl;
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

    public List<TaskVO> getTasks() {
        return tasks;
    }

    public void setTasks(List<TaskVO> tasks) {
        this.tasks = tasks;
    }

    @Override
    public String toString() {
        return "ProjectVO{" +
            "id=" + id +
            ", repoUrl='" + repoUrl + '\'' +
            ", sha='" + sha + '\'' +
            ", imageUrl='" + imageUrl + '\'' +
            ", apiUrl='" + apiUrl + '\'' +
            ", targetMilestone='" + targetMilestone + '\'' +
            ", userId=" + userId +
            ", tasks=" + tasks +
            '}';
    }
}
