<script setup>

import WifiIcon from "./components/WifiIcon.vue";
import CellularIcon from "./components/CellularIcon.vue";
import ConfigurationTabs from "./components/ConfigurationTabs.vue";
import ConnectivitySection from "./components/configuration-section/ConnectivitySection.vue";

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

      <ul class="nav col-12 col-md-auto mb-2 justify-content-center mb-md-0">
        <WifiIcon :connected="true" :rssi="-50"/>
        <CellularIcon :connected="true"/>
      </ul>

      <div class="col-md-3 text-end">
        Time: 15:15
      </div>
    </header>
  </div>
  <div class="container">
    <div class="row justify-content-center">
      <div class="col-10">
        <ConfigurationTabs current-tab="connectivity"/>
        <div class="tab-content">
          <div class="tab-pane fade show active" id="connectivity" role="tabpanel">
            <div class="container mt-3">
              <div class="row">
                <ConnectivitySection/>
              </div>
            </div>
          </div>
          <div class="tab-pane fade" id="nevym" role="tabpanel">nevym</div>
          <div class="tab-pane fade" id="kokod" role="tabpanel">kokod</div>
        </div>
      </div>
    </div>
  </div>
</template>

<style scoped>

</style>
