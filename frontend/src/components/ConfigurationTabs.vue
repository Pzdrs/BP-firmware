<script setup>
const props = defineProps({
  currentTab: String
});

const tabs = [
  {
    id: 'connectivity',
    label: 'Connectivity',
    icon: 'md-comparearrows'
  },
  {
    id: 'data-transmission',
    label: 'Data transmission',
    icon: 'md-comparearrows'
  }
];

function isCurrentTab(tabId) {
  return props.currentTab === tabId;
}

</script>

<template>
  <ul class="nav nav-tabs justify-content-center" id="tab">
    <li class="nav-item" role="presentation" v-for="tab in tabs" :key="tab.id">
      <button class="nav-link" :class="isCurrentTab(tab.id)?'active': ''" data-bs-toggle="tab"
              :data-bs-target="`#${tab.id}`" type="button"
              role="tab">
        <Icon :name="tab.icon" scale="1.25"/>
        {{ tab.label }}
      </button>
    </li>
  </ul>
</template>

<style scoped>

</style>
