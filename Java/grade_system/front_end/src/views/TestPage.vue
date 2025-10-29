<template>
  <div>
    <h1>{{ message}}这是测试页面</h1>
  </div>
  <p>{{ message }}</p>
</template>

<script setup>
import { ref } from 'vue'

const message = ref('hello world')
</script>
