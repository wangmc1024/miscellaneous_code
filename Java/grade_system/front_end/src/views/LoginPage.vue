<template>
  <div class="login-container">
    <h1>{{ $route.meta.title }}</h1>
    <form @submit.prevent="handleLogin">
      <div class="form-group">
        <label for="username">用户名：</label>
        <input id="username" v-model="loginForm.username" required />
      </div>
      <div class="form-group">
        <label for="password">密码：</label>
        <input id="password" type="password" v-model="loginForm.password" required />
      </div>
      <div class="form-group">
        <label for="role">身份：</label>
        <select id="role" v-model="loginForm.role">
          <option v-for="(r) in roles" :key="r.value" :value="r.value">{{ r.label }}</option>
        </select>
      </div>
      <button type="submit" :disabled="isLoading">登录</button>
    </form>
  </div>

   <!-- 霓虹弹窗遮罩（半透明背景） -->
  <div class="neon-overlay" :class="{ 'show': showNeonAlert }"></div>

  <!-- 霓虹弹窗主体 -->
  <div class="neon-alert" :class="{ 'show': showNeonAlert }">
    <div class="neon-title">欢迎回来！</div>
    <div class="neon-user">超级用户 _MC_</div>
    <button class="neon-btn" @click="goToHome">进入系统</button>
  </div>
</template>

<script setup>
import { ref } from 'vue'
import {useRouter} from 'vue-router'
import {useStore} from 'vuex'
 const router = useRouter()
 const store = useStore()

// 表单绑定数据
const loginForm = ref({
  username:'',
  password:'',
  role: 'teacher'
})

// 身份下拉框选项
const roles = [
  { label: '老师', value: 'teacher' },
  { label: '班长', value: 'monitor' },
  { label: '学生', value: 'student' }
]

const isLoading = ref(false)
const errorMessage = ref('')
const showNeonAlert = ref(false)

const goToHome = () => {
  showNeonAlert.value = false
  const redirect = router.currentRoute.value.query.redirect || `/${loginForm.value.role}`
  router.push(redirect)
}

const handleLogin = async () => {
  isLoading.value = true
  try {
    const result = await store.dispatch('user/login', loginForm.value)
    if (result.success && loginForm.value.username==='wangmc') {
      showNeonAlert.value = true

    }else if(result.success){
      const redirect = router.currentRoute.value.query.redirect || `/${loginForm.value.role}`
      router.push(redirect)
      console.log('success:log in')
    }
    else {
      errorMessage.value = result.message
      alert('用户名或密码错误，请重新输入')
    }
  } catch (error) {
    errorMessage.value = '登录过程中发生错误'
    console.error(error)
  } finally {
    isLoading.value = false
  }
}
</script>

<style scoped>
.login-container {
  max-width: 350px;
  margin: 60px auto;
  padding: 32px 24px;
  border: 1px solid #eee;
  border-radius: 8px;
  background: #fafbfc;
  box-shadow: 0 2px 8px #eee;
}
h1 {
  text-align: center;
  margin-bottom: 24px;
  color: #333;
}
.form-group {
  margin-bottom: 18px;
}
label {
  display: block;
  margin-bottom: 6px;
  font-weight: 500;
  color: #666;
}
input, select {
  width: 100%;
  padding: 9px 12px;
  border: 1px solid #ccc;
  border-radius: 4px;
  box-sizing: border-box;
  font-size: 14px;
  transition: border-color 0.2s;
}
input:focus, select:focus {
  outline: none;
  border-color: #409eff;
}
button {
  width: 100%;
  padding: 11px 0;
  background: #409eff;
  color: #fff;
  border: none;
  border-radius: 4px;
  font-size: 16px;
  cursor: pointer;
  transition: background-color 0.2s;
}
button:hover {
  background: #307ae8;
}

/* 遮罩层 */
.neon-overlay {
  position: fixed;
  top: 0;
  left: 0;
  width: 100vw;
  height: 100vh;
  background: rgba(0, 0, 0, 0.8);
  z-index: 9998;
  opacity: 0;
  visibility: hidden;
  transition: opacity 0.5s ease;
}

.neon-overlay.show {
  opacity: 1;
  visibility: visible;
}

/* 霓虹弹窗 */
.neon-alert {
  position: fixed;
  top: 50%;
  left: 50%;
  transform: translate(-50%, -50%) scale(0.9);
  width: 420px;
  padding: 2rem;
  background: #000;
  border: 2px solid #ff00ff;
  border-radius: 16px;
  box-shadow: 0 0 20px #ff00ff, 0 0 40px rgba(255, 0, 255, 0.5);
  z-index: 9999;
  opacity: 0;
  visibility: hidden;
  transition: all 0.5s ease;
  /* 弹窗闪烁动画 */
  animation: neonPulse 2s infinite alternate;
}

.neon-alert.show {
  opacity: 1;
  visibility: visible;
  transform: translate(-50%, -50%) scale(1);
}

/* 标题 */
.neon-title {
  font-size: 1.8rem;
  color: #fff;
  text-align: center;
  margin-bottom: 1.2rem;
  text-shadow: 0 0 10px #00ffff;
}

/* 用户名 */
.neon-user {
  font-size: 2.2rem;
  font-weight: bold;
  color: #ff00ff;
  text-align: center;
  margin-bottom: 2rem;
  text-shadow: 0 0 15px #ff00ff, 0 0 30px rgba(255, 0, 255, 0.7);
  /* 文字闪烁动画 */
  animation: textBlink 1.5s infinite alternate;
}

/* 按钮 */
.neon-btn {
  display: block;
  width: 100%;
  padding: 1rem;
  background: #ff00ff;
  border: none;
  border-radius: 8px;
  font-size: 1.1rem;
  font-weight: bold;
  color: #000;
  cursor: pointer;
  box-shadow: 0 0 15px #ff00ff;
  transition: all 0.3s ease;
}

.neon-btn:hover {
  background: #00ffff;
  box-shadow: 0 0 20px #00ffff;
}

</style>

<style>
/* 全局 keyframes 放在非 scoped 样式块中以避免 scoped 解析问题 */

@keyframes neonPulse {
  0% {
    box-shadow: 0 0 15px #ff00ff, 0 0 30px rgba(255, 0, 255, 0.4);
  }
  100% {
    box-shadow: 0 0 25px #ff00ff, 0 0 50px rgba(255, 0, 255, 0.8);
  }
}

@keyframes textBlink {
  0% {
    color: #ff66ff;
    text-shadow: 0 0 10px #ff66ff;
  }
  100% {
    color: #ff00ff;
    text-shadow: 0 0 20px #ff00ff;
  }
}
</style>
