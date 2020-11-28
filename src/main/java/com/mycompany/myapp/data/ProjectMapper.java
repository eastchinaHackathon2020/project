package com.mycompany.myapp.data;

import com.mycompany.myapp.domain.Project;
import org.apache.ibatis.annotations.Mapper;
import org.springframework.stereotype.Repository;

@Mapper
@Repository
public interface ProjectMapper {
    int addProject(Project project);
}
