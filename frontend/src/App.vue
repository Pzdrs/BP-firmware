<script setup>
import ConfigurationTabs from "./components/ConfigurationTabs.vue";
import ConnectivitySection from "./components/configuration-section/ConnectivitySection.vue";
import StatusBar from "./components/StatusBar.vue";
import TimeStatus from "./components/TimeStatus.vue";
import RestartButton from "./components/RestartButton.vue";
import GNSSSection from "./components/configuration-section/GNSSSection.vue";


const currentTab = 'connectivity';
const sections = [
  {
    id: 'connectivity',
    label: 'Connectivity',
    icon: 'md-comparearrows',
    component: ConnectivitySection
  },
  {
    id: 'gnss',
    label: 'GNSS',
    icon: 'md-gpsfixed',
    component: GNSSSection
  }
];

function isCurrentTab(tabId) {
  return currentTab === tabId;
}
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
        <ul class="nav nav-tabs justify-content-center" id="tab">
          <li class="nav-item" role="presentation" v-for="tab in sections" :key="tab.id">
            <button class="nav-link" :class="isCurrentTab(tab.id)?'active': ''" data-bs-toggle="tab"
                    :data-bs-target="`#${tab.id}`" type="button"
                    role="tab">
              <Icon :name="tab.icon" scale="1.25"/>
              {{ tab.label }}
            </button>
          </li>
        </ul>
        <div class="tab-content">
          <div v-for="section in sections"
               :key="section.id"
               class="tab-pane fade" :class="section.id === currentTab?'show active':''" :id="section.id"
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
