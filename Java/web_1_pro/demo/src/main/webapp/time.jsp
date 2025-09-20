<%@ page import="java.time.LocalDateTime" %>
<%@ page import="java.time.format.DateTimeFormatter" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title>当前时间显示</title>
    <style>
        body {
            font-family: 'Segoe UI', Arial, sans-serif;
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            height: 100vh;
            margin: 0;
            background-color: #f0f2f5;
        }
        .time-container {
            background-color: white;
            padding: 2rem 4rem;
            border-radius: 12px;
            box-shadow: 0 4px 20px rgba(0, 0, 0, 0.1);
            text-align: center;
        }
        .time-display {
            font-size: 2.5rem;
            font-weight: 300;
            color: #1a73e8;
            margin: 1rem 0;
        }
        .date-display {
            font-size: 1.2rem;
            color: #5f6368;
        }
        .info {
            margin-top: 1.5rem;
            color: #80868b;
            font-size: 0.9rem;
        }
    </style>
</head>
<body>
    <div class="time-container">
        <h1>当前时间</h1>
        <div class="date-display">
            <% 
                
                LocalDateTime now = LocalDateTime.now();
                
                DateTimeFormatter dateFormatter = DateTimeFormatter.ofPattern("yyyy年MM月dd日 EEEE");
                out.println(now.format(dateFormatter));
            %>
        </div>
       
        <div class="time-display" id="currentTime">
            <% 
                // 定义时间格式
                DateTimeFormatter timeFormatter = DateTimeFormatter.ofPattern("HH:mm:ss");
                out.println(now.format(timeFormatter));
            %>
        </div>
        <div class="info">
            时间每秒自动更新
        </div>
    </div>

    <script>
        
        function updateTime() {
            const now = new Date();
            
            const hours = String(now.getHours()).padStart(2, '0');
            const minutes = String(now.getMinutes()).padStart(2, '0');
            const seconds = String(now.getSeconds()).padStart(2, '0');
            const timeString = `${hours}:${minutes}:${seconds}`;
            
            document.getElementById('currentTime').textContent = timeString;
        }
        
        
        setInterval(updateTime, 1000);
    </script>
</body>
</html>
    