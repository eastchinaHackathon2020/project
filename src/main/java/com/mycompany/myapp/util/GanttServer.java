package com.mycompany.myapp.util;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONArray;
import com.alibaba.fastjson.JSONObject;
import com.mycompany.myapp.vo.Commit;

import java.io.*;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class GanttServer {

    public static String getImageUrl(String apiUrl,String targets) {

        String pathroot = "D:\\JavaSpace\\华东hackathon2020\\SEexperiement-master\\json\\";
///       String baseFileName = extractMessage("https://api.github.com/repos/eastchinaHackathon2020/testcase/commits", "1-商业模式;2-需求获取;3-需求规格;4-体系结构;5-前端;6-后端;");
        String baseFileName = extractMessage(apiUrl, targets);
        doPy("D:\\JavaSpace\\华东hackathon2020\\SEexperiement-master\\src\\main\\java\\com\\mycompany\\myapp\\util\\gantt.py",pathroot+baseFileName+".json");
        //用来返回前端
        String imageUrl  = OSSServer.uploadImage(pathroot+baseFileName+".png");
        System.out.println(imageUrl);
        return imageUrl;
    }

    /**
     *
     * @param pyPath
     * @param jsonPath
     */
    public static void doPy(String pyPath,String jsonPath){
        try {
            System.out.println("start");
            String[] args1=new String[]{"python",pyPath,jsonPath};
            Process pr=Runtime.getRuntime().exec(args1);
            InputStream is = pr.getInputStream();
            DataInputStream dis = new DataInputStream(is);
            String str = dis.readLine();
            pr.waitFor();
            System.out.println(str);
//            BufferedReader in = new BufferedReader(new InputStreamReader(
//                    pr.getInputStream()));
//            String line;
//            while ((line = in.readLine()) != null) {
//                System.out.println(line);
//            }
//            in.close();
//            pr.waitFor();
            System.out.println("end");
        } catch (Exception e) {
            e.printStackTrace();
        }}

    static List<Commit> getCommits(String apiUrl) {
        try {
            int page = 1;
            int per_page = 30;
            List<Commit> commits = new ArrayList<>();
            JSONArray jsonArray;
            do {
                String res = HttpClient4.doGet(apiUrl + "?page=" + page + "&per_page=" + per_page + "&access_token=c23bf38ff1be1b9fd73e91a757181314b8e702ce");
                System.out.println(res);
                page++;
                jsonArray = JSONArray.parseArray(res);
                System.out.println(jsonArray.size());
                for (int i = 0; i < jsonArray.size(); i++) {
                    
                    
                    JSONObject jsonObject = jsonArray.getJSONObject(i);
                    JSONObject commit = jsonObject.getJSONObject("commit");
                    JSONObject committer = commit.getJSONObject("author");
                    String date = committer.getObject("date", String.class);
                    String name = committer.getObject("name", String.class);
                    String message = commit.getObject("message", String.class);
                    Commit commit1 = new Commit();
                    commit1.setAuthor(name);
                    commit1.setDate(date);
                    commit1.setMessage(message);
                    commits.add(commit1);

                    System.out.println(message);
                }
            } while (jsonArray.size() != 0);
            System.out.println(commits.size() + " commits");
            return commits;
        } catch (Exception e) {
            return new ArrayList<>();
        }
    }

   static String extractMessage(String apiUrl, String targetMilestone) {

        ArrayList<String> colors=new ArrayList();
        colors.add( "lightgreen");
        colors.add( "orange");
        colors.add("blue");

        HashMap<String,String> colorMap = new HashMap<>();

        int colorIDX = 0;

        String a[] = apiUrl.split("/");
        String baseFileName = a[a.length-2];
        List<Commit> commits = getCommits(apiUrl);

        HashMap<String, Object> jsonTemp = new HashMap<>();// 准备转为json
        jsonTemp.put("title", "compilers");
        jsonTemp.put("xlabel", "time(commits)");
        ArrayList<HashMap<String,Object>> packages = new ArrayList();
        jsonTemp.put("packages", packages);
        ArrayList<Integer> xticks = new ArrayList<>();
//        //TODO 横坐标
//        for (int i = 2; i < 50; i++) {
//            xticks.add(i);
//        }
        jsonTemp.put("xticks",xticks);
        System.out.println("*************************************************");

        System.out.println(commits);

        System.out.println("*************************************************");

        int count = 0;

        Commit previous = new Commit();
        String[] targets = targetMilestone.split(";");
        for (int i = commits.size() - 1; i > 0; i--) {
            Commit commit = commits.get(i);
            for (String t : targets) {

                if (commit.getMessage().contains(t)) {
                    HashMap<String,Object> items = new HashMap<>();

                    ArrayList<Integer> ticks = new ArrayList<>();
                    items.put("milestones",ticks);
                    items.put("idx",0);
                    if(i!=commits.size() - 1){
                        if(!commit.getDate().substring(0,10).equals(previous.getDate().substring(0,10))){
                            count+=1;
                        }
                    }

                    int flag = 0;
                    for (int j=0;j<packages.size();j++)
                    {
                        HashMap<String,Object> pkg = packages.get(j);
                        //label 相同
                        if (((String)pkg.get("label")).substring(0,((String)pkg.get("label")).length()-2).equals(t)) {
                            //相同label 连着的 ,不另起一行
                            if (((Integer) pkg.get("end")) >= count) {
                                items.put("start", pkg.get("start"));
                                items.put("end", count + 1);
                                //更新milestonge
                                for (int k = 0; k < ((ArrayList<Integer>) pkg.get("milestones")).size(); k++) {
//                                    if(((ArrayList<Integer>) pkg.get("milestones")).get(k) >= count){
//                                        continue;
//                                    }

                                    ticks.add(
                                            ((ArrayList<Integer>) pkg.get("milestones")).get(k)
                                    );
                                }
//                                ticks.add(count + 1);
                                items.put("idx",(Integer)pkg.get("idx"));
                                packages.remove(pkg);

                            }
                            else{
                                // 相同label 不连着，另起一行
                                items.put("start",count);
                                items.put("end",count+1);
                                items.put("idx",(Integer)pkg.get("idx")+1);

                            }
                            flag = 1;
                        }
                    }

                    items.put("milestones",ticks);
                    items.put("label",t+"-"+(Integer)items.get("idx"));

                    if (colorMap.containsKey(commit.getAuthor())){
                        items.put("color",colorMap.get(commit.getAuthor()));
                    }
                    else {
                        colorMap.put(commit.getAuthor(),colors.get(colorIDX));
                        colorIDX++;
                        items.put("color",colorMap.get(commit.getAuthor()));
                    }

                    if (flag==0){
                        items.put("start",count);
                        items.put("end",count+1);
                    }
                    System.out.println(t + " by " + commit.getAuthor() + " at " + commit.getDate());
                    //identify milestone
//                    if (commit.getMessage().contains("1-overview: init")){
                    if (commit.getMessage().contains("+")){
                        ticks.add(count+1);
                        items.put("milestones", ticks);
                    }


                    packages.add(items);
                }
            }
            previous = commit;
        }
//        xticks.add(1);
//        xticks.add(count+20);
        packages.sort(new myHashSort());
        String json = JSON.toJSONString(jsonTemp);
        // 标记文件生成是否成功
        boolean flag2 = true;
        // 生成json格式文件
        try {
            // 保证创建一个新文件
            File file = new File("json/"+baseFileName+".json");//todo
            if(!file.exists()){
                try {
                    file.createNewFile();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            if (file.exists()) { // 如果已存在,删除旧文件
                file.delete();
            }
            file.createNewFile();
            // 将格式化后的字符串写入文件
            Writer write = new OutputStreamWriter(new FileOutputStream(file), "UTF-8");
            write.write(json);
            write.flush();
            write.close();
        } catch (Exception e) {

            flag2 = false;
            e.printStackTrace();
        }

        System.out.println(json);
        return baseFileName;
    }
}
