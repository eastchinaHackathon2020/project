package com.mycompany.myapp.data;

import com.mycompany.myapp.domain.Project;
import com.mycompany.myapp.vo.TaskVO;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.springframework.stereotype.Repository;

import java.util.List;

@Mapper
@Repository
public interface ProjectMapper {
    int addProject(Project project);
    List<Project> getProjectByUserId(@Param("userId") String userId);
    int addTask(TaskVO task);
    List<TaskVO> getTaskByProjectId(@Param("projectId") String projectId);
}
