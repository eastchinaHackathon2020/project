package com.mycompany.myapp.controller;

import com.mycompany.myapp.domain.Record;
import com.mycompany.myapp.repository.RecordRepository;
import com.mycompany.myapp.service.IUploadService;
//import com.mycompany.myapp.vo.Chunk;
import com.mycompany.myapp.service.impl.CmdServiceImpl;
import com.mycompany.myapp.vo.RecVO;
import com.mycompany.myapp.vo.RecordVO;
import lombok.extern.slf4j.Slf4j;
import org.apache.tomcat.util.http.fileupload.FileUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.multipart.MultipartFile;

import javax.servlet.http.HttpServletRequest;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.text.SimpleDateFormat;
import java.util.*;

@RestController
@RequestMapping("/upload")
@Slf4j
@CrossOrigin
@Service
public class UploadController {

    //private final static String CHUNK_FOLDER = "/Users/yangwei/resource/data/chunk";
    private final static String SINGLE_FOLDER = "E:\\work\\resource";

    @Autowired
    private IUploadService uploadService;
    @Autowired
    private RecordRepository recordRepository;
//    @Autowired
//    private CmdService cmdService;


//    @PostMapping("single")
//    public void singleUpload(Chunk chunk) {
//        MultipartFile file = chunk.getFile();
//        String filename = chunk.getFilename();
//        try {
//            byte[] bytes = file.getBytes();
//            if (!Files.isWritable(Paths.get(SINGLE_FOLDER))) {
//                Files.createDirectories(Paths.get(SINGLE_FOLDER));
//            }
//            Path path = Paths.get(SINGLE_FOLDER,filename);
//            Files.write(path, bytes);
//        } catch (IOException e) {
//            e.printStackTrace();
//        }
//    }


    @ResponseBody
    @RequestMapping("/single")
    public RecordVO uploadCategory(HttpServletRequest request,
                                   @RequestParam("file") MultipartFile[] file){
    Path path=null;
    String name="Tmaybe2free.cpp";
    String content="";
    if (file != null && file.length > 0) {
        for (MultipartFile temp : file) {
            try {
                byte[] bytes = temp.getBytes();
                if (!Files.isWritable(Paths.get(SINGLE_FOLDER))) {
                    Files.createDirectories(Paths.get(SINGLE_FOLDER));
                }
                path = Paths.get(SINGLE_FOLDER,temp.getOriginalFilename());
                name=temp.getOriginalFilename();
                Files.write(path, bytes);
                //System.out.println(new String(bytes));
                content=new String(bytes);
                Path solution_path=Paths.get(SINGLE_FOLDER,temp.getOriginalFilename()+"_solution.txt");
                Files.write(solution_path,bytes);
            } catch (IOException e) {
                e.printStackTrace();
            }
            }
        }
    String f=String.valueOf(path);
    String out="yyy";
    CmdServiceImpl.putFile(CmdServiceImpl.login("139.224.134.61","root","123561795Ljj"),f,"/root/ASTVisitor/build");
    out=CmdServiceImpl.execute(CmdServiceImpl.login("139.224.134.61","root","123561795Ljj"),"./ASTVisitor/build/myASTVisitor ./ASTVisitor/build/"+name);
    System.out.print(out);

    RecordVO recordVO=new RecordVO();
    recordVO.setContent(content);
    recordVO.setWarning(out);

        Date date=new Date();
        Calendar calendar = Calendar.getInstance();
        calendar.setTime(date);
        calendar.add(calendar.DATE, 0);//如果把0修改为-1就代表昨天
        date = calendar.getTime();
        SimpleDateFormat format= new SimpleDateFormat("yyyy-MM-dd");
        String dateString = format.format(date);
        System.out.println(dateString);
    Record record=new Record(4,out,name,content,dateString);
    recordRepository.save(record);
    return recordVO;
    }


    /**
    @GetMapping("chunk")
    public Map<String, Object> checkChunks(Chunk chunk) {
        return uploadService.checkChunkExits(chunk);
    }

    @PostMapping("chunk")
    public Map<String, Object> saveChunk(Chunk chunk) {
        MultipartFile file = chunk.getFile();
        Integer chunkNumber = chunk.getChunkNumber();
        String identifier = chunk.getIdentifier();
        byte[] bytes;
        try {
            bytes = file.getBytes();
            Path path = Paths.get(generatePath(CHUNK_FOLDER, chunk));
            Files.write(path, bytes);
        } catch (IOException e) {
            e.printStackTrace();
        }
        Integer chunks = uploadService.saveChunk(chunkNumber, identifier);
        Map<String, Object> result = new HashMap<>();
        if (chunks.equals(chunk.getTotalChunks())) {
            result.put("message","上传成功！");
            result.put("code", 205);
        }
        return result;
    }
    **/
}
