<script setup>
import ConfigurationTabs from "./components/ConfigurationTabs.vue";
import ConnectivitySection from "./components/configuration-section/ConnectivitySection.vue";
import StatusBar from "./components/StatusBar.vue";
import TimeStatus from "./components/TimeStatus.vue";
import RestartButton from "./components/RestartButton.vue";


const defaultSection = 'connectivity';
const sections = [
  {
    id: 'connectivity',
    component: ConnectivitySection
  }
];


</script>

<template>
  <div class="container-fluid">
    <header
        class="d-flex flex-wrap align-items-center justify-content-center justify-content-md-between py-3 px-5 mb-4 border-bottom">
      <div class="col-md-3 mb-2 mb-md-0">
        <a href="/" class="d-inline-flex link-body-emphasis text-decoration-none">
          ESP GPS
        </a>
      </div>
      <StatusBar/>
      <div class="col-md-3 text-end">
        <RestartButton class="me-1"/>
        <TimeStatus/>
      </div>
    </header>
  </div>
  <div class="container">
    <div class="row justify-content-center">
      <div class="col-10">
        <ConfigurationTabs current-tab="connectivity"/>
        <div class="tab-content">
          <div v-for="section in sections"
               :key="section.id"
               class="tab-pane fade" :class="section.id === defaultSection?'show active':''" :id="section.id"
               role="tabpanel">
            <div class="container mt-3">
              <div class="row">
                <Component :is="section.component"/>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<style scoped>

</style>
