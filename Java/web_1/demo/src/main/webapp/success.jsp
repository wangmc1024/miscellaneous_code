<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ page import="com.example.bean.User" %>
<html>
<head>
    <title>登录成功</title>
</head>
<body>
    <h3>登录成功！</h3>
    <%
        // 从 session 中获取用户对象
        User user = (User) session.getAttribute("user");
    %>
    <p>欢迎您，<%= user.getUsername() %></p>
    <a href="login.jsp">返回登录</a>
</body>
</html>