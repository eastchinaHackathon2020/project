package com.mycompany.myapp.vo;

public class ResponseVO {
    Boolean success;
    String message;
    Object content;

    public static ResponseVO buildSuccess(Object content){
        ResponseVO responseVO = new ResponseVO();
        responseVO.setSuccess(true);
        responseVO.setContent(content);
        return responseVO;
    }

    public static ResponseVO buildFailure(String message){
        ResponseVO responseVO = new ResponseVO();
        responseVO.setSuccess(false);
        responseVO.setMessage(message);
        return responseVO;
    }



    public Boolean getSuccess() {
        return success;
    }

    public void setSuccess(Boolean success) {
        this.success = success;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    public Object getContent() {
        return content;
    }

    public void setContent(Object content) {
        this.content = content;
    }
}
