<template>
  <div class="student-management">
    <h2>学生管理系统</h2>

    <!-- 加载状态 -->
    <div v-if="loading" class="loading">加载中...</div>

    <!-- 错误提示 -->
    <div v-else-if="error" class="error">{{ error }}</div>

    <!-- 主内容 -->
    <div v-else class="content">
      <!-- 搜索和添加按钮 -->
      <div class="toolbar">
        <input
          type="text"
          v-model="searchQuery"
          placeholder="搜索学生..."
          class="search-input"
          @input="handleSearch"
        />
        <button @click="showAddForm = true" class="add-btn">添加学生</button>
      </div>

      <!-- 学生列表 -->
      <div class="student-list">
        <table>
          <thead>
            <tr>
              <th>ID</th>
              <th>姓名</th>
              <th>成绩</th>
              <th>班级</th>
              <th>操作</th>
            </tr>
          </thead>
          <tbody>
            <tr v-for="student in paginatedStudents" :key="student.id">
              <td>{{ student.id }}</td>
              <td>{{ student.name }}</td>
              <td>{{ student.grade }}</td>
              <td>{{ student.className }}</td>
              <td class="actions">
                <button @click="editStudent(student)" class="edit-btn">编辑</button>
                <button @click="deleteStudent(student.id)" class="delete-btn">删除</button>
              </td>
            </tr>
          </tbody>
        </table>

        <!-- 空状态 -->
        <div v-if="students.length === 0" class="empty-state">
          暂无学生数据
        </div>
      </div>

      <!-- 分页 -->
      <div class="pagination" v-if="totalCount > 0">
        <button @click="prevPage" :disabled="currentPage === 1">上一页</button>
        <span>第 {{ currentPage }} 页，共 {{ totalPages }} 页</span>
        <span>共 {{ totalCount }} 条记录</span>
        <button @click="nextPage" :disabled="currentPage === totalPages">下一页</button>
        <div class="page-size">
          每页显示：
          <select v-model="pageSize" @change="handlePageSizeChange">
            <option value="5">5</option>
            <option value="10">10</option>
            <option value="20">20</option>
            <option value="50">50</option>
          </select>
        </div>
      </div>
    </div>

    <!-- 添加/编辑对话框 -->
    <div v-if="showAddForm || editingStudent" class="modal-overlay" @click="closeModal">
      <div class="modal" @click.stop>
        <h3>{{ editingStudent ? '编辑学生' : '添加学生' }}</h3>

        <form @submit.prevent="handleSubmit">
          <div class="form-group">
            <label>姓名：</label>
            <input
              type="text"
              v-model="formData.name"
              required
              placeholder="请输入学生姓名"
            />
          </div>

          <div class="form-group">
            <label>成绩：</label>
            <input
              type="number"
              v-model.number="formData.grade"
              required
              min="0"
              max="100"
              placeholder="请输入学生成绩"
            />
          </div>

          <div class="form-group">
            <label>班级：</label>
            <input
              type="text"
              v-model="formData.className"
              required
              placeholder="请输入班级名称"
            />
          </div>

          <div class="form-actions">
            <button type="button" @click="closeModal">取消</button>
            <button type="submit" :disabled="saving">
              {{ saving ? '保存中...' : '保存' }}
            </button>
          </div>
        </form>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref, computed, onMounted } from 'vue'
import { useStudentStore } from '@/api/students'

const {
  students,
  loading,
  error,
  totalCount,
  currentPage,
  pageSize,
  paginatedStudents,
  fetchAllStudents,
  createStudent,
  updateStudent,
  deleteStudent: deleteStudentApi,
  prevPage,
  nextPage,
} = useStudentStore()

const showAddForm = ref(false)
const editingStudent = ref(null)
const saving = ref(false)
const searchQuery = ref('')
const formData = ref({
  name: '',
  className: '',
  grade: ''
})

const totalPages = computed(() => {
  return Math.ceil(totalCount.value / pageSize.value)
})

const filteredStudents = computed(() => {
  if (!searchQuery.value.trim()) {
    return students.value
  }

  const query = searchQuery.value.toLowerCase().trim()
  return students.value.filter(student =>
    student.name.toLowerCase().includes(query) ||
    student.className.toLowerCase().includes(query) ||
    String(student.id).includes(query)
  )
})

const loadData = async () => {
  await fetchAllStudents()
}

onMounted(() => {
  loadData()
})

const handleSearch = () => {
  if (currentPage.value !== 1) {
    currentPage.value = 1
  }
  if(searchQuery.value.trim()){
    students.value = filteredStudents.value
  }else{
    loadData()
  }
}

const handlePageSizeChange = () => {
  currentPage.value = 1
}

const editStudent = async (student) => {
  editingStudent.value = student
  formData.value = {
    name: student.name,
    grade: student.grade,
    className: student.className
  }
}

const closeModal = () => {
  showAddForm.value = false
  editingStudent.value = null
  formData.value = {
    name: '',
    grade: '',
    className: ''
  }
}

const handleSubmit = async () => {
  saving.value = true

  try {
    if (editingStudent.value) {
      await updateStudent(editingStudent.value.id, formData.value)
    } else {
      await createStudent(formData.value)
    }

    closeModal()
    console.log('操作成功')
  } catch (err) {
    console.error('操作失败:', err)
  } finally {
    saving.value = false
  }
}

const deleteStudent = async (studentId) => {
  if (confirm('确定要删除这个学生吗？')) {
    try {
      await deleteStudentApi(studentId)
      console.log('删除成功')
    } catch (err) {
      console.error('删除失败:', err)
    }
  }
}
</script>

<style scoped>
.student-management {
  font-family: Arial, sans-serif;
  max-width: 1200px;
  margin: 0 auto;
  padding: 20px;
}

h2 {
  color: #333;
  margin-bottom: 20px;
}

.loading,
.error {
  padding: 20px;
  text-align: center;
  border-radius: 4px;
  margin-bottom: 20px;
}

.loading {
  background-color: #f0f0f0;
  color: #666;
}

.error {
  background-color: #ffebee;
  color: #d32f2f;
}

.toolbar {
  display: flex;
  gap: 10px;
  margin-bottom: 20px;
}

.search-input {
  flex: 1;
  padding: 8px 12px;
  border: 1px solid #ddd;
  border-radius: 4px;
  font-size: 14px;
}

.add-btn,
.edit-btn,
.delete-btn {
  padding: 8px 16px;
  border: none;
  border-radius: 4px;
  cursor: pointer;
  font-size: 14px;
  transition: background-color 0.2s;
}

.add-btn {
  background-color: #4caf50;
  color: white;
}

.add-btn:hover {
  background-color: #45a049;
}

.edit-btn {
  background-color: #2196f3;
  color: white;
  margin-right: 8px;
}

.edit-btn:hover {
  background-color: #0b7dda;
}

.delete-btn {
  background-color: #f44336;
  color: white;
}

.delete-btn:hover {
  background-color: #da190b;
}

.student-list {
  margin-bottom: 20px;
}

table {
  width: 100%;
  border-collapse: collapse;
}

th, td {
  padding: 12px;
  text-align: left;
  border-bottom: 1px solid #ddd;
}

th {
  background-color: #f5f5f5;
  font-weight: bold;
}

.empty-state {
  text-align: center;
  padding: 40px;
  color: #666;
}

.pagination {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-top: 20px;
}

.pagination button {
  padding: 6px 12px;
  border: 1px solid #ddd;
  background-color: white;
  cursor: pointer;
  border-radius: 4px;
}

.pagination button:disabled {
  opacity: 0.5;
  cursor: not-allowed;
}

.page-size select {
  padding: 4px;
  border: 1px solid #ddd;
  border-radius: 4px;
}

.modal-overlay {
  position: fixed;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background-color: rgba(0, 0, 0, 0.5);
  display: flex;
  justify-content: center;
  align-items: center;
  z-index: 1000;
}

.modal {
  background-color: white;
  padding: 24px;
  border-radius: 8px;
  width: 100%;
  max-width: 500px;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
}

.modal h3 {
  margin-top: 0;
  margin-bottom: 20px;
  color: #333;
}

.form-group {
  margin-bottom: 16px;
}

.form-group label {
  display: block;
  margin-bottom: 4px;
  font-weight: bold;
  color: #555;
}

.form-group input {
  width: 100%;
  padding: 8px 12px;
  border: 1px solid #ddd;
  border-radius: 4px;
  font-size: 14px;
  box-sizing: border-box;
}

.form-actions {
  display: flex;
  gap: 10px;
  justify-content: flex-end;
  margin-top: 24px;
}

.form-actions button {
  padding: 8px 16px;
  border: 1px solid #ddd;
  border-radius: 4px;
  cursor: pointer;
  font-size: 14px;
}

.form-actions button[type="button"] {
  background-color: #f5f5f5;
}

.form-actions button[type="submit"] {
  background-color: #4caf50;
  color: white;
  border-color: #4caf50;
}

.form-actions button:disabled {
  opacity: 0.5;
  cursor: not-allowed;
}

.actions {
  white-space: nowrap;
}
</style>
