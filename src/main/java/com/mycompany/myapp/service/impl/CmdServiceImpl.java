package com.mycompany.myapp.service.impl;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;

import ch.ethz.ssh2.SCPClient;
import org.apache.commons.lang.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import ch.ethz.ssh2.Connection;
import ch.ethz.ssh2.Session;
import ch.ethz.ssh2.StreamGobbler;

public class CmdServiceImpl {

    private static final Logger log = LoggerFactory.getLogger(CmdServiceImpl.class);
    private static String  DEFAULTCHART="UTF-8";

    /**
     * 登录主机
     * @return
     *      登录成功返回true，否则返回false
     */
    public static Connection login(String ip,
                                   String userName,
                                   String userPwd){

        boolean flg=false;
        Connection conn = null;
        try {
            conn = new Connection(ip);
            conn.connect();//连接
            flg=conn.authenticateWithPassword(userName, userPwd);//认证
            if(flg){
                log.info("=========登录成功========="+conn);
                return conn;
            }
        } catch (IOException e) {
            log.error("=========登录失败========="+e.getMessage());
            e.printStackTrace();
        }
        return conn;
    }

    /**
     * 远程执行shll脚本或者命令
     * @param cmd
     *      即将执行的命令
     * @return
     *      命令执行完后返回的结果值
     */
    public static String execute(Connection conn,String cmd){
        String result="";
        try {
            if(conn !=null){
                Session session= conn.openSession();//打开一个会话
                session.execCommand(cmd);//执行命令
                result=processStdout(session.getStdout(),DEFAULTCHART);
                //如果为得到标准输出为空，说明脚本执行出错了
                if(StringUtils.isBlank(result)){
                    log.info("得到标准输出为空,链接conn:"+conn+",执行的命令："+cmd);
                    result=processStdout(session.getStderr(),DEFAULTCHART);
                    //getFile(conn,"E:/","/home/hsl/ASTVisitor/build/warn.dat");
                }else{
                    log.info("执行命令成功,链接conn:"+conn+",执行的命令："+cmd);
                    //getFile(conn,"E:/","/home/hsl/ASTVisitor/build/warn.dat");
                }
                conn.close();
                session.close();
            }
        } catch (IOException e) {
            log.info("执行命令失败,链接conn:"+conn+",执行的命令："+cmd+"  "+e.getMessage());
            e.printStackTrace();
        }
        return result;
    }
    /**
     * 解析脚本执行返回的结果集
     * @param in 输入流对象
     * @param charset 编码
     * @return
     *       以纯文本的格式返回
     */
    private static String processStdout(InputStream in, String charset){
        InputStream  stdout = new StreamGobbler(in);
        StringBuffer buffer = new StringBuffer();;
        try {
            BufferedReader br = new BufferedReader(new InputStreamReader(stdout,charset));
            String line=null;
            while((line=br.readLine()) != null){
                if(line.startsWith("./ASTVisitor")&& !line.contains("fatal error")){
                    line=line.substring(19);
                    int pos=Integer.parseInt(line.split(":")[3]);
                    String error_info="";
                    String extra_info="";
                    switch (pos){
                        case 0:
                            error_info="数组越界:";
                            break;
                        case 1:
                            error_info="慢速内存:";
                            break;
                        case 2:
                            error_info="类型不匹配:";
                            break;
                        case 3:
                            error_info="大型变量";
                            extra_info="大小--"+line.split(":")[4]+"MB";
                            break;
                        case 4:
                            error_info="空指针";
                            extra_info="指针名--"+line.split(":")[4];
                            break;
                        case 5:
                            error_info="double free";
                            extra_info="指针名--"+line.split(":")[4];
                    }
                    String sp []=line.split(":");
                    sp[3]=error_info;
                    if(pos>=3) sp[4]=extra_info;
                    line=String.join(":",sp);
                    buffer.append(line+"\n");
                }
            }
        } catch (UnsupportedEncodingException e) {
            log.error("解析脚本出错："+e.getMessage());
            e.printStackTrace();
        } catch (IOException e) {
            log.error("解析脚本出错："+e.getMessage());
            e.printStackTrace();
        }
        return buffer.toString();
    }

    public static void putFile(Connection conn, String fileName, String remotePath){
        SCPClient sc = new SCPClient(conn);
        try {
            //将本地文件放到远程服务器指定目录下，默认的文件模式为 0600，即 rw，
            //如要更改模式，可调用方法 put(fileName, remotePath, mode),模式须是4位数字且以0开头
            sc.put(fileName, remotePath);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void getFile(Connection conn, String local, String remotePath){
        SCPClient sc = new SCPClient(conn);
        try {
            //将本地文件放到远程服务器指定目录下，默认的文件模式为 0600，即 rw，
            //如要更改模式，可调用方法 put(fileName, remotePath, mode),模式须是4位数字且以0开头
            sc.get(remotePath, local);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
