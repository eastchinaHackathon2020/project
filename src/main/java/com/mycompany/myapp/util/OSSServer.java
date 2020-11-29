package com.mycompany.myapp.util;

import com.aliyun.oss.OSSClient;
import com.aliyun.oss.model.ObjectMetadata;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;

public class OSSServer {
    private static final String accessKeyId = "LTAI4G8wqGH4HdWASLz5Ziic";
    private static final String accessKeySecret = "11bpMAvbBroFCjZ80xKOHktmWo6iJH";
    private static final String endpoint = "oss-cn-beijing.aliyuncs.com";
    private static final String bucketName = "gitvisual";



//    public static void main(String[] args) {
//        String objectName = "D:\\JavaSpace\\华东hackathon2020\\hackthonPNG\\hackthonPNG\\test\\testcase.png";
//        System.out.println(uploadImage(objectName));
//
//    }
    public static String uploadImage(String filePath){
//        String objectName = "C:\\Users\\admin\\Desktop\\test.png";
        String objectName = filePath;
        // 创建OSSClient实例。
        OSSClient ossClient = new OSSClient(endpoint, accessKeyId, accessKeySecret);
        // 上传文件
        ObjectMetadata meta = new ObjectMetadata();
        meta.setContentType("image/jpg");
        meta.setCacheControl("no-store");
        // 上传文件流。
        InputStream inputStream = null;
        try {
            inputStream = new FileInputStream(objectName);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }

        //文件名
        String strs[] = filePath.split("/|\\\\");// todo maybe

        String baseFileName = strs[strs.length-1];//有 png的

        ossClient.putObject(bucketName, baseFileName, inputStream,meta);
        // 关闭OSSClient。
        ossClient.shutdown();
        return "https://gitvisual.oss-cn-beijing.aliyuncs.com/"+baseFileName;
    }
}

