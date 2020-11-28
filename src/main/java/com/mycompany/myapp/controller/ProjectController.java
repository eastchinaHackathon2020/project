package com.mycompany.myapp.controller;

import com.mycompany.myapp.service.IProjectService;
import com.mycompany.myapp.vo.FormVO;
import com.mycompany.myapp.vo.ResponseVO;
import org.apache.ibatis.annotations.Param;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/project")
public class ProjectController {
    @Autowired
    IProjectService projectService;

    @PostMapping("registerProject")
    public ResponseVO registerProject(@RequestBody FormVO formVO){
        return projectService.registerProject(formVO);
    }

    @GetMapping("userProject/{userId}")
    public ResponseVO getUserProject(@PathVariable("userId") String userId){
        return projectService.userProject(userId);
    }

    @RequestMapping(value = { "test" }, method = { RequestMethod.GET }, produces="image/svg+xml; charset=utf-8")
    public String test(){
        return "<h3>hello</h3>";
    }
}
