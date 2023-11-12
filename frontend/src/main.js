import {createApp} from 'vue'
import 'bootstrap/dist/css/bootstrap.css'
import 'bootstrap/js/dist/tab.js'
import 'bootstrap/js/dist/collapse.js'
import 'bootstrap/js/dist/modal.js'
import App from './App.vue'
import OhVueIcon from './icons'

const app = createApp(App)

app.component('Icon', OhVueIcon)

app.mount('#app');
