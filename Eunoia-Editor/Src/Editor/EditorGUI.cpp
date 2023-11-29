#include "EditorGUI.h"

#include "../../Eunoia-Engine/Src/Vendor/imgui/imgui.h"
#include "../../Eunoia-Engine/Src/Vendor/imgui/imgui_internal.h"
#include "../../Eunoia-Engine/Src/Vendor/imgui/imgui_impl_opengl3.h"
#include "../../Eunoia-Engine/Src/Vendor/imgui/imgui_impl_win32.h"

#include <Eunoia\Platform\Windows\DisplayWin32.h>
#include <Eunoia\Platform\OpenGL\RenderAPIOpenGL.h>
#include <shellapi.h>

#include "ProjectManager.h"

#define EDITOR_GENERAL_NAME_BUFFER_SIZE 128
#define EDITOR_FILE_NAME_BUFFER_SIZE 64
#define EDITOR_ENTITY_NAME_BUFFER_SIZE 64

enum AssetModify
{
	ASSET_MODIFY_RENAME,
	ASSET_MODIFY_DELETE,
	ASSET_MODIFY_NONE
};

enum ECSFilter
{
	ECS_FILTER_ALL,
	ECS_FILTER_ENGINE,
	ECS_FILTER_PROJECT,
	NUM_ECS_FILTER_OPTIONS
};

struct EditorGUI_Data
{
	ImGuiContext* context;

	String popup;

	EntityID selectedEntity;

	EUDirectory* currentDirectory;
	EUDirectory* previousDirectory;

	AssetModify dirModify;
	AssetModify fileModify;
	u32 assetModifyIndex;

	TextureID gameWindowTexture;
	TextureID directoryIcon;
	TextureID fileIcon;

	char generalNameBuffer[EDITOR_GENERAL_NAME_BUFFER_SIZE];
	char fileNameBuffer[EDITOR_FILE_NAME_BUFFER_SIZE];
	char entityNameBuffer[EDITOR_ENTITY_NAME_BUFFER_SIZE];
};

static EditorGUI_Data s_EditorGUI;

void EditorGUI::Init()
{
	s_EditorGUI.selectedEntity = EU_ECS_INVALID_ENTITY_ID;
	s_EditorGUI.dirModify = ASSET_MODIFY_NONE;
	s_EditorGUI.fileModify = ASSET_MODIFY_NONE;
	s_EditorGUI.gameWindowTexture = EU_INVALID_TEXTURE_ID;

	DisplayWin32* display = (DisplayWin32*)Engine::GetDisplay();
	HWND handle = display->GetHandle();
	s_EditorGUI.context = ImGui::CreateContext();
	ImGui::SetCurrentContext(s_EditorGUI.context);

	ImGuiIO& io = ImGui::GetIO();
	ImGuiInitTheme::InitDark();
	u32 dwidth, dheight;
	display->GetSize(&dwidth, &dheight);
	io.DisplaySize.x = dwidth;
	io.DisplaySize.y = dheight;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	
	io.FontDefault = io.Fonts->AddFontFromFileTTF("Res/Fonts/OpenSans-Regular.ttf", 18.0f);

	ImGui_ImplWin32_InitForOpenGL(handle);
	ImGui_ImplOpenGL3_Init("#version 330");

	s_EditorGUI.directoryIcon = Engine::GetApplication()->GetActiveScene()->assetManager->LoadTexture("Res/Textures/DirectoryIcon.eutex");
	s_EditorGUI.fileIcon = Engine::GetApplication()->GetActiveScene()->assetManager->LoadTexture("Res/Textures/FileIcon.eutex");
}

void EditorGUI::Begin()
{
	ImGui::SetCurrentContext(s_EditorGUI.context);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void EditorGUI::End()
{
	ImGui::Render();
	ImGui::UpdatePlatformWindows();
}

static void DrawEditableText(String* text, char* charBuffer, u32 charBufferSize)
{
	EU_PERSISTENT b32 RenamingText = false;

	if (RenamingText)
	{
		if (ImGui::InputText("##DET_NameInput", charBuffer, charBufferSize, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
		{
			*text = charBuffer;
			RenamingText = false;
			s_EditorGUI.generalNameBuffer[0] = 0;
		}
	}
	else
	{
		ImGui::Text(text->C_Str());
	}


	if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(EU_BUTTON_LEFT) && !RenamingText)
	{
		RenamingText = true;
		memcpy(charBuffer, text->C_Str(), text->Length());
		charBuffer[text->Length()] = 0;
	}
}

static void DrawPopups()
{
	if (!s_EditorGUI.popup.Empty())
		ImGui::OpenPopup(s_EditorGUI.popup.C_Str());

	s_EditorGUI.popup = "";

	if (ImGui::BeginPopupModal("Create Project"))
	{
		ImGui::InputText("##Create Project_Name", s_EditorGUI.generalNameBuffer, EDITOR_GENERAL_NAME_BUFFER_SIZE);

		if (ImGui::Button("Cancel##Create Project_Cancel"))
		{
			s_EditorGUI.generalNameBuffer[0] = 0;
			ImGui::CloseCurrentPopup();
		}ImGui::SameLine();
		if (ImGui::Button("Create##Create Project_Create"))
		{
			ProjectManager::CreateNewProject(s_EditorGUI.generalNameBuffer);
			s_EditorGUI.currentDirectory = ProjectManager::GetProject()->assetDirectory;
			s_EditorGUI.previousDirectory = s_EditorGUI.currentDirectory;
			s_EditorGUI.generalNameBuffer[0] = 0;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
	if (ImGui::BeginPopupModal("Load Project"))
	{
		const List<String>& projects = ProjectManager::GetProjectNamesInProjectFolder();
		EU_PERSISTENT s32 SelectedProject = -1;
		EU_PERSISTENT b32 CompileProject = false;
		if (ImGui::BeginCombo("##Load Project_Combo", (SelectedProject == -1) ? "Select Project" : projects[SelectedProject].C_Str()))
		{
			for (u32 i = 0; i < projects.Size(); i++)
			{
				if (ImGui::Selectable(projects[i].C_Str()))
				{
					SelectedProject = i;
				}
			}
			ImGui::EndCombo();

		}

		if (SelectedProject != -1)
		{
			String text = "Compile " + projects[SelectedProject];

			ImGui::Text(text.C_Str()); ImGui::SameLine();
			ImGui::Checkbox("##Load Project_CompileCB",(bool*)&CompileProject);

			if (ImGui::Button("Cancel##Load Project_Cancel1"))
			{
				ImGui::CloseCurrentPopup();
			} ImGui::SameLine();
			if (ImGui::Button("Load##Load Project_Load"))
			{
				ProjectManager::LoadProject(projects[SelectedProject], CompileProject);
				s_EditorGUI.currentDirectory = ProjectManager::GetProject()->assetDirectory;
				s_EditorGUI.previousDirectory = s_EditorGUI.currentDirectory;
				ImGui::CloseCurrentPopup();
			}
		}
		else
		{
			if (ImGui::Button("Cancel##Load Project_Cancel2"))
			{
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::EndPopup();
	}
	if (ImGui::BeginPopupModal("Delete Project"))
	{
		ImGui::EndPopup();
	}
	if (ImGui::BeginPopup("Modify Directory"))
	{
		if (ImGui::Selectable("Rename"))
		{
			s_EditorGUI.dirModify = ASSET_MODIFY_RENAME;
			ImGui::CloseCurrentPopup();
		} ImGui::Separator();
		if (ImGui::Selectable("Delete"))
		{
			s_EditorGUI.dirModify = ASSET_MODIFY_DELETE;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	if (ImGui::BeginPopup("Modify File"))
	{
		if (ImGui::Selectable("Rename"))
		{
			s_EditorGUI.fileModify = ASSET_MODIFY_RENAME;
			ImGui::CloseCurrentPopup();
		} ImGui::Separator();
		if (ImGui::Selectable("Delete"))
		{
			s_EditorGUI.fileModify = ASSET_MODIFY_DELETE;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	if (ImGui::BeginPopup("Create Asset"))
	{
		if (ImGui::Selectable("New Folder"))
		{
			String newFolderName = "New Folder(" + String::S32ToString(s_EditorGUI.currentDirectory->directories.Size()) + ")";
			s_EditorGUI.currentDirectory->CreateNewDir(newFolderName);
			ImGui::CloseCurrentPopup();
			s_EditorGUI.dirModify = ASSET_MODIFY_RENAME;
			s_EditorGUI.assetModifyIndex = s_EditorGUI.currentDirectory->directories.Size() - 1;
		}ImGui::Separator();
		if (ImGui::Selectable("New File"))
		{
			String newFileName = "New File(" + String::S32ToString(s_EditorGUI.currentDirectory->files.Size()) + ").txt";
			s_EditorGUI.currentDirectory->CreateNewFile(newFileName);
			ImGui::CloseCurrentPopup();
			s_EditorGUI.fileModify = ASSET_MODIFY_RENAME;
			s_EditorGUI.assetModifyIndex = s_EditorGUI.currentDirectory->files.Size() - 1;
		}ImGui::Separator();
		if (ImGui::Selectable("New Scene"))
		{
			String newSceneName = "New Scene( " + String::S32ToString(s_EditorGUI.currentDirectory->files.Size()) + ").euscn";
			s_EditorGUI.currentDirectory->CreateNewFile(newSceneName);
			EUFile* createdFile = &s_EditorGUI.currentDirectory->files.GetLastElement();
			s_EditorGUI.fileModify = ASSET_MODIFY_RENAME;
			s_EditorGUI.assetModifyIndex = s_EditorGUI.currentDirectory->files.Size() - 1;
			
			u32 numScenesInDirectory = 0;
			for (u32 i = 0; i < s_EditorGUI.currentDirectory->files.Size(); i++)
			{
				if (s_EditorGUI.currentDirectory->files[i].extension == "euscn")
					numScenesInDirectory++;
			}

			Application* app = ProjectManager::GetProject()->application;

			ECSLoadedScene loadedScene;
			loadedScene.name = "Scene";
			//if (numScenesInDirectory > 0)
				//loadedScene.name += String::S32ToString(numScenesInDirectory);
			
			ECSLoadedEntity rootEntity;
			rootEntity.numChildren = 0;
			rootEntity.enabled = true;
			rootEntity.name = "Root";

			ECSLoadedSystem requiredSystem;
			requiredSystem.typeID = Metadata::GetTypeID<TransformHierarchy2DSystem>();
			requiredSystem.enabled = true;

			loadedScene.entities.Push(rootEntity);
			loadedScene.systems.Push(requiredSystem);

			ECSLoader::WriteLoadedSceneToFile(loadedScene, createdFile->path);

			ImGui::CloseCurrentPopup();
		}ImGui::Separator();
		if (ImGui::Selectable("Open in File Explorer"))
		{
			ShellExecuteA(NULL, "open", s_EditorGUI.currentDirectory->GetAbsPath().C_Str() , NULL, NULL, SW_SHOWDEFAULT);
		}
		ImGui::EndPopup();
	}
	if (ImGui::BeginPopup("SceneViewer_AddContent"))
	{
		if (ImGui::Selectable("Add Entity"))
		{
			ECS * ecs = ProjectManager::GetProject()->application->GetActiveScene()->ecs;
			EntityID entity = ecs->CreateEntity("New Entity", true, EU_ECS_ROOT_ENTITY);
			String entityName = "New Entity (" + String::S32ToString(entity) + ")";
			ecs->SetEntityName(entity, entityName);
			ImGui::CloseCurrentPopup();
		}
		ImGui::Separator();
		if (ImGui::Selectable("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	if (ImGui::BeginPopup("Entity_Add_Content"))
	{
		ECS* ecs = ProjectManager::GetProject()->application->GetActiveScene()->ecs;
		if (ImGui::Selectable("+Components"))
		{
			s_EditorGUI.popup = "Add Components";
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Selectable("+Child"))
		{
			String entityName = ecs->GetEntityName(s_EditorGUI.selectedEntity) + "-Child";
			u32 numChildren = ecs->GetAllEntities_()[s_EditorGUI.selectedEntity - 1].children.Size();
			entityName = entityName + String::S32ToString(numChildren);
			s_EditorGUI.selectedEntity = ecs->CreateEntity(entityName, true, s_EditorGUI.selectedEntity);
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Selectable("-Delete"))
		{
			ecs->DestroyEntity(s_EditorGUI.selectedEntity);
		}
		ImGui::EndPopup();
	}
	if (ImGui::BeginPopupModal("Add Components"))
	{
		EU_PERSISTENT ECSFilter Filter = ECS_FILTER_ALL;

		ImGui::InputText("##AddComponents_SearchBar", s_EditorGUI.generalNameBuffer, EDITOR_GENERAL_NAME_BUFFER_SIZE);
		ImGui::SameLine();

		String previewValue = "UNDEFINED";
		if (Filter == ECS_FILTER_ALL)
			previewValue = "Show All Components";
		if (Filter == ECS_FILTER_ENGINE)
			previewValue = "Show Engine Components";
		if (Filter == ECS_FILTER_PROJECT)
			previewValue = "Show Project Components";

		if (ImGui::BeginCombo("##AddComponents_Filter", previewValue.C_Str()))
		{
			if (ImGui::Selectable("Show All Components##EEFilterShowAll"))
			{
				Filter = ECS_FILTER_ALL;
			}
			if (ImGui::Selectable("Show Engine Components##EEFilterShowAll"))
			{
				Filter = ECS_FILTER_ENGINE;
			}
			if (ImGui::Selectable("Show Project Components##EEFilterShowAll"))
			{
				Filter = ECS_FILTER_PROJECT;
			}
			ImGui::EndCombo();
		}

		ECS* ecs = ProjectManager::GetProject()->application->GetActiveScene()->ecs;

		const List<MetadataInfo>& componentTypes = Metadata::GetComponentMetadataList();
		EU_PERSISTENT List<b32> SelectedComponents;
		EU_PERSISTENT b32 InitSelectedComponents = true;

		if (InitSelectedComponents)
		{
			SelectedComponents.SetCapacityAndElementCount(componentTypes.Size());
			for (u32 i = 0; i < SelectedComponents.Size(); i++)
				SelectedComponents[i] = false;

			InitSelectedComponents = false;
		}

		if (Filter == ECS_FILTER_ALL ||
			Filter == ECS_FILTER_ENGINE)
		{
			ImGui::Text("Engine Components");
			ImGui::Separator();

			ImGui::Columns(3, 0, false);
			for (u32 i = 0; i < componentTypes.Size(); i++)
			{
				if (componentTypes[i].id <= Metadata::LastEngineTypeID)
				{
					String name = componentTypes[i].cls->name + "##AddComponents" + componentTypes[i].cls->name;
					ImGui::Checkbox(name.C_Str(), (bool*)&SelectedComponents[i]);
				}
				ImGui::NextColumn();
			}
			ImGui::EndColumns();
		}

		if (Filter == ECS_FILTER_ALL ||
			Filter == ECS_FILTER_PROJECT)
		{
			ImGui::Text("Project Components");
			ImGui::Separator();

			ImGui::Columns(3, 0, false);
			for (u32 i = 0; i < componentTypes.Size(); i++)
			{
				if (componentTypes[i].id > Metadata::LastEngineTypeID)
				{
					String name = componentTypes[i].cls->name + "##AddComponents" + componentTypes[i].cls->name;
					ImGui::Checkbox(name.C_Str(), (bool*)&SelectedComponents[i]);
				}
				ImGui::NextColumn();
			}
			ImGui::EndColumns();
		}

		if (ImGui::Button("Cancel##Add Components_CancelB"))
		{
			ImGui::CloseCurrentPopup();
			InitSelectedComponents = true;
		} ImGui::SameLine();
		if (ImGui::Button("Create##Add Components_CreateB"))
		{
			for (u32 i = 0; i < SelectedComponents.Size(); i++)
			{
				if (SelectedComponents[i])
				{
					ecs->CreateComponent(s_EditorGUI.selectedEntity, componentTypes[i].id, true);
				}
			}
			InitSelectedComponents = true;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
	if (ImGui::BeginPopupModal("Add Systems"))
	{
		EU_PERSISTENT ECSFilter Filter = ECS_FILTER_ALL;

		ImGui::InputText("##AddSystems_SearchBar", s_EditorGUI.generalNameBuffer, EDITOR_GENERAL_NAME_BUFFER_SIZE);
		ImGui::SameLine();

		String previewValue = "UNDEFINED";
		if (Filter == ECS_FILTER_ALL)
			previewValue = "Show All Systems";
		if (Filter == ECS_FILTER_ENGINE)
			previewValue = "Show Engine Systems";
		if (Filter == ECS_FILTER_PROJECT)
			previewValue = "Show Project Systems";

		if (ImGui::BeginCombo("##AddSystems_Filter", previewValue.C_Str()))
		{
			if (ImGui::Selectable("Show All Systems##EEFilterShowAll"))
			{
				Filter = ECS_FILTER_ALL;
			}
			if (ImGui::Selectable("Show Engine Systems##EEFilterShowAll"))
			{
				Filter = ECS_FILTER_ENGINE;
			}
			if (ImGui::Selectable("Show Project Systems##EEFilterShowAll"))
			{
				Filter = ECS_FILTER_PROJECT;
			}
			ImGui::EndCombo();
		}

		ECS* ecs = ProjectManager::GetProject()->application->GetActiveScene()->ecs;

		const List<MetadataInfo>& systemTypes = Metadata::GetSystemMetadataList();
		EU_PERSISTENT List<b32> SelectedSystems;
		EU_PERSISTENT b32 InitSelectedSystems = true;

		if (InitSelectedSystems)
		{
			SelectedSystems.SetCapacityAndElementCount(systemTypes.Size());
			for (u32 i = 0; i < SelectedSystems.Size(); i++)
				SelectedSystems[i] = false;

			InitSelectedSystems = false;
		}

		if (Filter == ECS_FILTER_ALL ||
			Filter == ECS_FILTER_ENGINE)
		{
			ImGui::Text("Engine Systems");
			ImGui::Separator();

			ImGui::Columns(3, 0, false);
			for (u32 i = 0; i < systemTypes.Size(); i++)
			{
				if (systemTypes[i].id <= Metadata::LastEngineTypeID)
				{
					String name = systemTypes[i].cls->name + "##AddSystems" + systemTypes[i].cls->name;
					ImGui::Checkbox(name.C_Str(), (bool*)&SelectedSystems[i]);
				}
				ImGui::NextColumn();
			}
			ImGui::EndColumns();
		}

		if (Filter == ECS_FILTER_ALL ||
			Filter == ECS_FILTER_PROJECT)
		{
			ImGui::Text("Project Systems");
			ImGui::Separator();

			ImGui::Columns(3, 0, false);
			for (u32 i = 0; i < systemTypes.Size(); i++)
			{
				if (systemTypes[i].id > Metadata::LastEngineTypeID)
				{
					String name = systemTypes[i].cls->name + "##AddComponents" + systemTypes[i].cls->name;
					ImGui::Checkbox(name.C_Str(), (bool*)&SelectedSystems[i]);
				}
				ImGui::NextColumn();
			}
			ImGui::EndColumns();
		}

		if (ImGui::Button("Cancel##Add Systems_CancelB"))
		{
			ImGui::CloseCurrentPopup();
			InitSelectedSystems = true;
		} ImGui::SameLine();
		if (ImGui::Button("Create##Add Systems_CreateB"))
		{
			for (u32 i = 0; i < SelectedSystems.Size(); i++)
			{
				if (SelectedSystems[i])
				{
					ecs->CreateSystem(systemTypes[i].id, true);
				}
			}
			InitSelectedSystems = true;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
	if (ImGui::BeginPopup("Systems Add Content"))
	{
		if (ImGui::Selectable("+ Systems"))
		{
			s_EditorGUI.popup = "Add Systems";
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

static void DrawMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Project"))
		{
			if (ImGui::Selectable("Create Project"))
			{
				s_EditorGUI.popup = "Create Project";
			}
			if (ImGui::Selectable("Load Project"))
			{
				s_EditorGUI.popup = "Load Project";
			}
			if (ImGui::Selectable("Save Project"))
			{
				ProjectManager::SaveProject();
			}
			ImGui::Separator();
			if (ImGui::Selectable("Open VS2019"))
			{
				ProjectManager::OpenVS2019();
			}
			ImGui::Separator();
			if (ImGui::Selectable("Delete Project"))
			{
				s_EditorGUI.popup = "Delete Project";
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

static void DrawSceneEntityHelper(EntityID entityID, List<ECSEntityContainer>& entities)
{
	ECSEntityContainer* entity = &entities[entityID - 1];
	if (!entity->initialized)
		return;

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	if (entityID == s_EditorGUI.selectedEntity)
	{
		flags |= ImGuiTreeNodeFlags_Selected;
	}
	b32 opened = ImGui::TreeNodeEx((entity->name + "##SV" + entity->name).C_Str(), flags);

	if (ImGui::IsItemHovered() && (ImGui::IsMouseClicked(EU_BUTTON_LEFT) || ImGui::IsMouseClicked(EU_BUTTON_RIGHT)))
	{
		s_EditorGUI.selectedEntity = entityID;
		memcpy(s_EditorGUI.entityNameBuffer, entity->name.C_Str(), entity->name.Length() + 1);

		if (ImGui::IsMouseClicked(EU_BUTTON_RIGHT))
		{
			s_EditorGUI.popup = "Entity_Add_Content";
		}
	}

	if (opened)
	{
		for (u32 i = 0; i < entity->children.Size(); i++)
		{
			DrawSceneEntityHelper(entity->children[i], entities);
		}

		ImGui::TreePop();
	}

}

static void DrawSceneViewer()
{
	if (ImGui::Begin("Scene Viewer"))
	{
		EunoiaProject* project = ProjectManager::GetProject();
		if (!project->loaded)
		{
			ImGui::End();
			return;
		}

		if (!project->application->HasActiveScene())
		{
			ImGui::Text("No Active Scene");
			ImGui::End();
			return;
		}
		
		EU_PERSISTENT b32 DrawRootNode = true;

		Scene* scene = ProjectManager::GetProject()->application->GetActiveScene();
		
		DrawEditableText(&scene->name, s_EditorGUI.generalNameBuffer, EDITOR_GENERAL_NAME_BUFFER_SIZE);
		
		if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(EU_BUTTON_RIGHT))
		{
			s_EditorGUI.popup = "SceneViewer_AddContent";
		}
		
		List<ECSEntityContainer>& entities = scene->ecs->GetAllEntities_();
		
		ECSEntityContainer* root = &entities[EU_ECS_ROOT_ENTITY - 1];
		for (u32 i = 0; i < root->children.Size(); i++)
		{
			DrawSceneEntityHelper(root->children[i], entities);
		}
		
	}
	ImGui::End();
}

static void DrawMetadataMember(MetadataClass* cls, u32 memberIndex, const u8* data, const String& idString, b32 noEdit = false);

static void DrawMetadataHelper(const MetadataInfo& metadata, const u8* data, const String& idString)
{
	MetadataClass* cls = metadata.cls;
	for (u32 i = 0; i < cls->members.Size(); i++)
	{
		ImGui::Columns(2, 0, false);
		DrawMetadataMember(cls, i, data, idString);
		ImGui::EndColumns();
	}
}

static void DrawMetadataClass(const MetadataInfo& metadata, const u8* data, const String& idString, String customName = "")
{
	if (metadata.type == METADATA_CLASS)
	{
		MetadataClass* cls = metadata.cls;
		if (!customName.Empty())
		{
			if (ImGui::TreeNodeEx((customName + idString).C_Str()))
			{
				DrawMetadataHelper(metadata, data, idString + customName);
				ImGui::TreePop();
			}
		}
		else
		{
			if (ImGui::TreeNodeEx((cls->name + idString).C_Str()))
			{
				DrawMetadataHelper(metadata, data, idString);
				ImGui::TreePop();
			}
		}
	}
}

static void DrawMetadataMember(MetadataClass* cls, u32 memberIndex, const u8* data, const String& idString, b32 noEdit)
{
	const MetadataMember& member = cls->members[memberIndex];
	String memberName = member.name;
	const MetadataInfo& metadata = Metadata::GetMetadata(member.typeID);
	const u8* offsetedData = data + member.offset;

	
	if (metadata.type == METADATA_PRIMITIVE)
	{
		String newIDString = idString + member.name;

		if (member.typeName == "TextureID")
		{
			ImGui::Text(memberName.C_Str());
			ImGui::NextColumn();
			if (ImGui::ColorButton((newIDString + "TextureCB").C_Str(), ImVec4(1.0f, 1.0f, 1.0f, 1.0f)))
			{
				s_EditorGUI.popup = "Texture_Select";
			}

			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* assetFile = ImGui::AcceptDragDropPayload("AssetFile");

				if (assetFile)
				{
					u32 assetFileIndex = *(u32*)assetFile->Data;
					EUFile* file = &s_EditorGUI.currentDirectory->files[assetFileIndex];
					if (file->extension == "eutex")
					{
						TextureID loaddedTexture = ProjectManager::GetProject()->application->GetActiveScene()->assetManager->LoadTexture(file->path);
						*((TextureID*)offsetedData) = loaddedTexture;
					}
				}
			}

			ImGui::NextColumn();
		}
		else
		{
			if (member.isNoEdit || noEdit)
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			}

			const MetadataPrimitive* primitive = metadata.primitive;
			switch (primitive->type)
			{
			case METADATA_PRIMITIVE_BOOL: {
				bool* value = (bool*)offsetedData;
				ImGui::Text(memberName.C_Str()); ImGui::SameLine();
				ImGui::NextColumn();
				ImGui::Checkbox(newIDString.C_Str(), value);
				ImGui::NextColumn();
			} break;
			case METADATA_PRIMITIVE_R32: {
				r32* value = (r32*)offsetedData;
				ImGui::Text(memberName.C_Str()); ImGui::SameLine();
				ImGui::NextColumn();
				ImGui::DragFloat(newIDString.C_Str(), value, cls->members[memberIndex].uiSliderSpeed, cls->members[memberIndex].uiSliderMin.x, cls->members[memberIndex].uiSliderMax.x);
				ImGui::NextColumn();
			} break;
			case METADATA_PRIMITIVE_R64: {
				r64* value = (r64*)offsetedData;
				ImGui::Text(memberName.C_Str()); ImGui::SameLine();
				ImGui::NextColumn();
				ImGui::DragFloat(newIDString.C_Str(), (r32*)value, cls->members[memberIndex].uiSliderSpeed, cls->members[memberIndex].uiSliderMin.x, cls->members[memberIndex].uiSliderMax.x);
				ImGui::NextColumn();
			} break;
			case METADATA_PRIMITIVE_U8: {
				u8* value = (u8*)offsetedData;
				ImGui::Text(memberName.C_Str()); ImGui::SameLine();
				ImGui::NextColumn();
				ImGui::DragInt(newIDString.C_Str(), (s32*)value, cls->members[memberIndex].uiSliderSpeed, EU_MAX(cls->members[memberIndex].uiSliderMin.x, 0), EU_MIN(cls->members[memberIndex].uiSliderMax.x, EU_U8_MAX));
				ImGui::NextColumn();
			} break;
			case METADATA_PRIMITIVE_U16: {
				u16* value = (u16*)offsetedData;
				ImGui::Text(memberName.C_Str()); ImGui::SameLine();
				ImGui::NextColumn();
				ImGui::DragInt(newIDString.C_Str(), (s32*)value, cls->members[memberIndex].uiSliderSpeed, EU_MAX(cls->members[memberIndex].uiSliderMin.x, 0), EU_MIN(cls->members[memberIndex].uiSliderMax.x, EU_U16_MAX));
				ImGui::NextColumn();
			} break;
			case METADATA_PRIMITIVE_U32: {
				u32* value = (u32*)offsetedData;
				ImGui::Text(memberName.C_Str()); ImGui::SameLine();
				ImGui::NextColumn();
				if (member.is32BitBool)
					ImGui::Checkbox(newIDString.C_Str(), (bool*)value);
				else
					ImGui::DragInt(newIDString.C_Str(), (s32*)value, cls->members[memberIndex].uiSliderSpeed, EU_MAX(cls->members[memberIndex].uiSliderMin.x, 0), EU_MIN(cls->members[memberIndex].uiSliderMax.x, EU_U32_MAX));

				ImGui::NextColumn();
			} break;
			case METADATA_PRIMITIVE_S8: {
				s8* value = (s8*)offsetedData;
				ImGui::Text(memberName.C_Str()); ImGui::SameLine();
				ImGui::NextColumn();
				ImGui::DragInt(newIDString.C_Str(), (s32*)value, cls->members[memberIndex].uiSliderSpeed, EU_MAX(cls->members[memberIndex].uiSliderMin.x, 0), EU_MIN(cls->members[memberIndex].uiSliderMax.x, EU_S8_MAX));
				ImGui::NextColumn();
			} break;
			case METADATA_PRIMITIVE_S16: {
				s16* value = (s16*)offsetedData;
				ImGui::Text(memberName.C_Str()); ImGui::SameLine();
				ImGui::NextColumn();
				ImGui::DragInt(newIDString.C_Str(), (s32*)value, cls->members[memberIndex].uiSliderSpeed, EU_MAX(cls->members[memberIndex].uiSliderMin.x, 0), EU_MIN(cls->members[memberIndex].uiSliderMax.x, EU_S16_MAX));
				ImGui::NextColumn();
			} break;
			case METADATA_PRIMITIVE_S32: {
				s32* value = (s32*)offsetedData;
				ImGui::Text(memberName.C_Str()); ImGui::SameLine();
				ImGui::NextColumn();
				ImGui::DragInt(newIDString.C_Str(), (s32*)value, cls->members[memberIndex].uiSliderSpeed, EU_MAX(cls->members[memberIndex].uiSliderMin.x, 0), EU_MIN(cls->members[memberIndex].uiSliderMax.x, EU_S32_MAX));
				ImGui::NextColumn();
			} break;
			}

			if (member.isNoEdit || noEdit)
			{
				ImGui::PopItemFlag();
				ImGui::PopStyleVar();
			}
		}
	}
	else if (metadata.type == METADATA_CLASS)
	{
		MetadataClass* memberClass = metadata.cls;
		String newIDString = idString + member.name;

		if (member.isNoEdit || noEdit)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}

		if (cls->members[memberIndex].typeID == Metadata::GetTypeID<v2>())
		{
			ImGui::EndColumns();
			ImGui::Columns(3, 0, false);
			r32* xValue = (r32*)(offsetedData + memberClass->members[0].offset);
			r32* yValue = (r32*)(offsetedData + memberClass->members[1].offset);
			ImGui::Text((memberName + " ").C_Str()); ImGui::SameLine();
			ImGui::NextColumn();
			ImGui::Text("X"); ImGui::SameLine();
			ImGui::DragFloat((newIDString + "x").C_Str(), xValue, cls->members[memberIndex].uiSliderSpeed, cls->members[memberIndex].uiSliderMin.x, cls->members[memberIndex].uiSliderMax.x);
			ImGui::NextColumn();
			ImGui::Text("Y"); ImGui::SameLine();
			ImGui::DragFloat((newIDString + "y").C_Str(), yValue, cls->members[memberIndex].uiSliderSpeed, cls->members[memberIndex].uiSliderMin.y, cls->members[memberIndex].uiSliderMax.y);
			ImGui::EndColumns();
			ImGui::Columns(2, 0, false);
		}
		else if (cls->members[memberIndex].typeID == Metadata::GetTypeID<v3>())
		{
			ImGui::EndColumns();
			ImGui::Columns(4, 0, false);
			r32* xValue = (r32*)(offsetedData + memberClass->members[0].offset);
			r32* yValue = (r32*)(offsetedData + memberClass->members[1].offset);
			r32* zValue = (r32*)(offsetedData + memberClass->members[2].offset);
			ImGui::Text((memberName + " ").C_Str()); ImGui::SameLine();
			ImGui::NextColumn();
			ImGui::Text("X"); ImGui::SameLine();
			ImGui::DragFloat((newIDString + "x").C_Str(), xValue, cls->members[memberIndex].uiSliderSpeed, cls->members[memberIndex].uiSliderMin.x, cls->members[memberIndex].uiSliderMax.x);
			ImGui::NextColumn();
			ImGui::Text("Y"); ImGui::SameLine();
			ImGui::DragFloat((newIDString + "y").C_Str(), yValue, cls->members[memberIndex].uiSliderSpeed, cls->members[memberIndex].uiSliderMin.y, cls->members[memberIndex].uiSliderMax.y);
			ImGui::NextColumn();
			ImGui::Text("Z"); ImGui::SameLine();
			ImGui::DragFloat((newIDString + "z").C_Str(), zValue, cls->members[memberIndex].uiSliderSpeed, cls->members[memberIndex].uiSliderMin.y, cls->members[memberIndex].uiSliderMax.y);
			ImGui::EndColumns();
			ImGui::Columns(2, 0, false);
		}
		else if (cls->members[memberIndex].typeID == Metadata::GetTypeID<v4>())
		{
			ImGui::EndColumns();
			ImGui::Columns(5, 0, false);
			r32* xValue = (r32*)(offsetedData + memberClass->members[0].offset);
			r32* yValue = (r32*)(offsetedData + memberClass->members[1].offset);
			r32* zValue = (r32*)(offsetedData + memberClass->members[2].offset);
			r32* wValue = (r32*)(offsetedData + memberClass->members[3].offset);
			ImGui::Text((memberName + " ").C_Str()); ImGui::SameLine();
			ImGui::NextColumn();
			ImGui::Text("X"); ImGui::SameLine();
			ImGui::DragFloat((newIDString + "x").C_Str(), xValue, cls->members[memberIndex].uiSliderSpeed, cls->members[memberIndex].uiSliderMin.x, cls->members[memberIndex].uiSliderMax.x);
			ImGui::NextColumn();
			ImGui::Text("Y"); ImGui::SameLine();
			ImGui::DragFloat((newIDString + "y").C_Str(), yValue, cls->members[memberIndex].uiSliderSpeed, cls->members[memberIndex].uiSliderMin.y, cls->members[memberIndex].uiSliderMax.y);
			ImGui::NextColumn();
			ImGui::Text("Z"); ImGui::SameLine();
			ImGui::DragFloat((newIDString + "z").C_Str(), zValue, cls->members[memberIndex].uiSliderSpeed, cls->members[memberIndex].uiSliderMin.y, cls->members[memberIndex].uiSliderMax.y);
			ImGui::NextColumn();
			ImGui::Text("W"); ImGui::SameLine();
			ImGui::DragFloat((newIDString + "w").C_Str(), wValue, cls->members[memberIndex].uiSliderSpeed, cls->members[memberIndex].uiSliderMin.y, cls->members[memberIndex].uiSliderMax.y);
			ImGui::EndColumns();
			ImGui::Columns(2, 0, false);
		}
		else
		{
			if (member.isNoEdit || noEdit)
			{
				ImGui::PopItemFlag();
			}

			b32 opened = ImGui::TreeNodeEx((member.name + newIDString + "Dropdown").C_Str());

			if (member.isNoEdit || noEdit)
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			}

			if (opened)
			{
				DrawMetadataHelper(metadata, offsetedData, newIDString + "Dropdown");
				ImGui::TreePop();
				ImGui::Columns(2, 0, false);
			}
		}

		if (member.isNoEdit || noEdit)
		{
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}
	}
	else if (metadata.type == METADATA_ENUM)
	{
		MetadataEnum* enm = metadata.enm;
		String newIDString = idString + member.name;

		ImGui::EndColumns();
		ImGui::Columns(2, 0, false);

		if (member.isNoEdit || noEdit)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}

		ImGui::Text(memberName.C_Str());
		ImGui::NextColumn();

		String previewValue = "EU_UNKNOWN_VALUE";

		for (u32 i = 0; i < enm->values.Size(); i++)
		{
			if (*(u32*)offsetedData == enm->values[i].value)
			{
				previewValue = enm->values[i].name;
				break;
			}
		} 

		if (ImGui::BeginCombo((newIDString + "EnumSelected").C_Str(), previewValue.C_Str()))
		{
			for (u32 i = 0; i < enm->values.Size(); i++)
			{
				if (ImGui::Selectable((enm->values[i].name + newIDString + enm->values[i].name).C_Str()))
				{
					previewValue = enm->values[i].name;
					(*(u32*)offsetedData) = enm->values[i].value;
				}
			}
			ImGui::EndCombo();
		}

		ImGui::EndColumns();
		ImGui::Columns(2, 0, false);

		if (member.isNoEdit || noEdit)
		{
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}
	}
}

static void DrawEntityViewer()
{
	if (ImGui::Begin("Entity Viewer"))
	{
		EunoiaProject* project = ProjectManager::GetProject();
		if (!project->loaded)
		{
			ImGui::End();
			return;
		}

		if (!project->application->HasActiveScene())
		{
			ImGui::Text("No Active Scene");
			ImGui::End();
			return;
		}

		if (s_EditorGUI.selectedEntity == EU_ECS_INVALID_ENTITY_ID)
		{
			ImGui::Text("No Selected Entity");
			ImGui::End();
			return;
		}

		if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(EU_BUTTON_RIGHT))
		{
			s_EditorGUI.popup = "Entity_Add_Content";
		}

		ECS* ecs = ProjectManager::GetProject()->application->GetActiveScene()->ecs;
		List<ECSEntityContainer>& entities = ecs->GetAllEntities_();
		ECSEntityContainer* entity = &entities[s_EditorGUI.selectedEntity - 1];
		
		ImGuiInputTextFlags flags = ImGuiInputTextFlags_AutoSelectAll;
		if (ImGui::InputText("##EVEntityName", s_EditorGUI.entityNameBuffer, EDITOR_ENTITY_NAME_BUFFER_SIZE, flags))
		{
			entity->name = s_EditorGUI.entityNameBuffer;
		} ImGui::SameLine();
		if (ImGui::Button("Add Components##EVAddComponentsB"))
		{
			s_EditorGUI.popup = "Add Components";
		}

		for (u32 i = 0; i < entity->components.Size(); i++)
		{
			ECSComponentContainer* componentContainer = &entity->components[i];
			DrawMetadataClass(Metadata::GetMetadata(componentContainer->typeID), (u8*)componentContainer->actualComponent, "##EV");
		}
	}
	ImGui::End();
}

static void DrawSystemViewer()
{
	if (ImGui::Begin("System Viewer"))
	{
		EunoiaProject* project = ProjectManager::GetProject();
		if (!project->loaded)
		{
			ImGui::End();
			return;
		}

		if (!project->application->HasActiveScene())
		{
			ImGui::Text("No Active Scene");
			ImGui::End();
			return;
		}

		if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(EU_BUTTON_RIGHT))
		{
			s_EditorGUI.popup = "Systems Add Content";
		}

		if (ImGui::Button("Add Systems##SV_AdddSystemB"))
		{
			s_EditorGUI.popup = "Add Systems";
		}

		ECS* ecs = project->application->GetActiveScene()->ecs;
		List<ECSSystemContainer>& systems = ecs->GetSystems_();
		for (u32 i = 0; i < systems.Size(); i++)
		{
			const MetadataInfo& metadata = Metadata::GetMetadata(systems[i].typeID);
			DrawMetadataClass(metadata, (u8*)systems[i].actualSystem, "##SV");
		}
	}
	ImGui::End();
}

static void DrawAssetBrowser()
{
	if (ImGui::Begin("Asset Browser"))
	{
		if (!ProjectManager::GetProject()->loaded)
		{
			ImGui::End();
			return;
		}

		r32 padding = 8.0f;
		r32 thumbnailSize = 128;
		r32 cellSize = thumbnailSize + padding;
		r32 contentWidth = ImGui::GetContentRegionAvail().x;
		u32 columnCount = EU_MAX(contentWidth / cellSize, 1);

		if (ImGui::ArrowButton("AssetBrowsertBackDirButton", ImGuiDir_Left))
		{
			if (s_EditorGUI.currentDirectory->parentDirectory)
			{
				s_EditorGUI.previousDirectory = s_EditorGUI.currentDirectory;
				s_EditorGUI.currentDirectory = s_EditorGUI.currentDirectory->parentDirectory;
			}
		} ImGui::SameLine();
		if (ImGui::ArrowButton("AssetBrowserPrevButton", ImGuiDir_Right))
		{
			if (s_EditorGUI.previousDirectory)
				s_EditorGUI.currentDirectory = s_EditorGUI.previousDirectory;
		} ImGui::SameLine();
		if (ImGui::Button("Refresh##AssetBrowserRefresh"))
		{
			s_EditorGUI.currentDirectory->Refresh();
		}
		ImGui::Separator();

		EU_PERSISTENT b32 RenamingDirectory = false;
		EU_PERSISTENT b32 RenamingFile = false;
		if (s_EditorGUI.dirModify == ASSET_MODIFY_DELETE)
		{
			s_EditorGUI.currentDirectory->directories[s_EditorGUI.assetModifyIndex]->Delete();
			s_EditorGUI.dirModify = ASSET_MODIFY_NONE;
		}
		else if (s_EditorGUI.dirModify == ASSET_MODIFY_RENAME)
		{
			RenamingDirectory = true;
			s_EditorGUI.dirModify = ASSET_MODIFY_NONE;
			u32 ami = s_EditorGUI.assetModifyIndex;
			const String& fileName = s_EditorGUI.currentDirectory->directories[s_EditorGUI.assetModifyIndex]->name;
			memcpy(s_EditorGUI.fileNameBuffer, fileName.C_Str(), fileName.Length() + 1);
		}
		if (s_EditorGUI.fileModify == ASSET_MODIFY_DELETE)
		{
			s_EditorGUI.currentDirectory->files[s_EditorGUI.assetModifyIndex].Delete();
			s_EditorGUI.fileModify = ASSET_MODIFY_NONE;
		}
		else if (s_EditorGUI.fileModify == ASSET_MODIFY_RENAME)
		{
			RenamingFile = true;
			s_EditorGUI.fileModify = ASSET_MODIFY_NONE;
			const String& fileName = s_EditorGUI.currentDirectory->files[s_EditorGUI.assetModifyIndex].name;
			memcpy(s_EditorGUI.fileNameBuffer, fileName.C_Str(), fileName.Length() + 1);
		}

		ImGui::Columns(columnCount, 0, false);

		ImVec4 iconBgColor = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
		b32 iconRightClicked = false;
		EUDirectory* newDirectory = s_EditorGUI.currentDirectory;
		if (!s_EditorGUI.currentDirectory)
			return;

		for (u32 i = 0; i < s_EditorGUI.currentDirectory->directories.Size(); i++)
		{
			EUDirectory* dir = s_EditorGUI.currentDirectory->directories[i];
			ImGui::PushID(("AssetBrowserDirectory" + String::S32ToString(i)).C_Str());
			ImGui::ImageButton(s_EditorGUI.directoryIcon, ImVec2(thumbnailSize, thumbnailSize), ImVec2(0, 0), ImVec2(1, 1), 0, iconBgColor);

			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* assetFile = ImGui::AcceptDragDropPayload("AssetFile");
				const ImGuiPayload* assetDir = ImGui::AcceptDragDropPayload("AssetDir");

				if (assetFile)
				{
					u32 assetFileIndex = *(u32*)assetFile->Data;
					s_EditorGUI.currentDirectory->MoveFileIntoDir(i, assetFileIndex);
				}
				if (assetDir)
				{
					u32 assetDirIndex = *(u32*)assetDir->Data;
					s_EditorGUI.currentDirectory->MoveDirectory(i, assetDirIndex);
				}

				ImGui::EndDragDropTarget();
			}
			if (ImGui::BeginDragDropSource())
			{
				ImGui::ImageButton(s_EditorGUI.directoryIcon, ImVec2(thumbnailSize / 2, thumbnailSize / 2), ImVec2(0, 0), ImVec2(1, 1), 0, iconBgColor);
				ImGui::SetDragDropPayload("AssetDir", &i, sizeof(u32));
				ImGui::EndDragDropSource();
			}
			ImGui::PopID();
			if (ImGui::IsItemHovered())
			{
				if (ImGui::IsMouseDoubleClicked(EU_BUTTON_LEFT))
				{
					newDirectory = dir;
				}
				else if (ImGui::IsMouseClicked(EU_BUTTON_RIGHT))
				{
					iconRightClicked = true;
					s_EditorGUI.popup = "Modify Directory";
					s_EditorGUI.assetModifyIndex = i;
				}
			}

			if (RenamingDirectory && (s_EditorGUI.assetModifyIndex == i))
			{
				ImGui::SetKeyboardFocusHere(0);
				if (ImGui::InputText(("##AssetDirectoryRename" + String::S32ToString(i)).C_Str(), s_EditorGUI.fileNameBuffer, EDITOR_FILE_NAME_BUFFER_SIZE,
					ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll) ||
					((ImGui::IsMouseClicked(0) || ImGui::IsMouseClicked(1)) && !ImGui::IsItemHovered()))
				{
					RenamingDirectory = false;
					s_EditorGUI.assetModifyIndex = -1;
					if (s_EditorGUI.fileNameBuffer[0] != '\0')
						s_EditorGUI.currentDirectory->directories[i]->Rename(s_EditorGUI.fileNameBuffer);
				}
			}
			else
			{
				ImGui::Text(dir->name.C_Str());
			}

			ImGui::NextColumn();
		}

		for (u32 i = 0; i < s_EditorGUI.currentDirectory->files.Size(); i++)
		{
			EUFile* file = &s_EditorGUI.currentDirectory->files[i];
			ImGui::PushID(("AssetBrowserFile" + String::S32ToString(i)).C_Str());
			ImGui::ImageButton(s_EditorGUI.fileIcon, ImVec2(thumbnailSize, thumbnailSize), ImVec2(0, 0), ImVec2(1, 1), 0, iconBgColor);
			if (ImGui::BeginDragDropSource())
			{
				ImGui::ImageButton(s_EditorGUI.fileIcon, ImVec2(thumbnailSize / 2, thumbnailSize / 2), ImVec2(0, 0), ImVec2(1, 1), 0, iconBgColor, ImVec4(0.6f, 0.6f, 0.6f, 0.85f));
				ImGui::SetDragDropPayload("AssetFile", &i, sizeof(u32));
				ImGui::EndDragDropSource();
			}
			ImGui::PopID();
			if (ImGui::IsItemHovered())
			{
				if (ImGui::IsMouseDoubleClicked(EU_BUTTON_LEFT))
				{
					if (file->extension == "euscn")
					{
						Application* app = ProjectManager::GetProject()->application;
						b32 sceneAlreadyLoaded = false;
						for (u32 i = 0; i < app->m_Scenes_.Size(); i++)
						{
							if (app->m_Scenes_[i].path == file->path)
							{
								sceneAlreadyLoaded = true;
								app->m_ActiveScene_ = (SceneID)(i + 1);
							}
						}

						if (!sceneAlreadyLoaded)
						{
							ProjectManager::GetProject()->application->CreateScene(file->path, file->name, true);
						}
					}
				}
				else if (ImGui::IsMouseClicked(EU_BUTTON_RIGHT))
				{
					iconRightClicked = true;
					s_EditorGUI.popup = "Modify File";
					s_EditorGUI.assetModifyIndex = i;
				}
			}

			if (RenamingFile && (s_EditorGUI.assetModifyIndex == i))
			{
				ImGui::SetKeyboardFocusHere(0);
				if (ImGui::InputText(("##AssetFileRename" + String::S32ToString(i)).C_Str(), s_EditorGUI.fileNameBuffer, EDITOR_FILE_NAME_BUFFER_SIZE, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll) ||
					((ImGui::IsMouseClicked(0) || ImGui::IsMouseClicked(1)) && !ImGui::IsItemHovered()))
				{
					RenamingFile = false;
					s_EditorGUI.assetModifyIndex = -1;
					if (s_EditorGUI.fileNameBuffer[0] != '\0')
						s_EditorGUI.currentDirectory->files[i].Rename(s_EditorGUI.fileNameBuffer);
				}
			}
			else
			{
				ImGui::Text(file->name.C_Str());
			}

			ImGui::NextColumn();
		}

		s_EditorGUI.currentDirectory = newDirectory;

		ImGui::Columns(1, 0, false);

		if (ImGui::IsWindowHovered() & ImGui::IsMouseClicked(EU_BUTTON_RIGHT) && !iconRightClicked)
		{
			s_EditorGUI.popup = "Create Asset";
		}

	}
	ImGui::End();
}

static void DrawGameViewer()
{
	if (ImGui::Begin("Game Window"))
	{
		if (!ProjectManager::GetProject()->loaded)
		{
			ImGui::End();
			return;
		}

		ImVec2 windowSize = ImGui::GetWindowSize();
		windowSize.y -= 40;

		u32 width, height;
		TextureID outputTexture = Engine::GetRenderer()->GetOutputTexture();
		Engine::GetRenderAPI()->GetTextureSize(outputTexture, &width, &height);

		if (windowSize.x != width || windowSize.y != height)
		{
			Engine::GetRenderer()->ResizeOutput(windowSize.x, windowSize.y);
		}

		ImGui::Image(outputTexture, ImVec2(width, height));
	}
	ImGui::End();
}

static void DrawGUIHelper()
{
	DrawMainMenuBar();
	DrawSceneViewer();
	DrawSystemViewer();
	DrawEntityViewer();
	DrawAssetBrowser();
	DrawGameViewer();
	DrawPopups();
}

void EditorGUI::DrawGUI()
{
	u32 width, height;
	Engine::GetDisplay()->GetSize(&width, &height);

	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	bool open;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("EunoiaEditor", &open, window_flags);
	ImGui::PopStyleVar(3);

	ImGuiID dockspace_id = ImGui::GetID("EU");

	if (ImGui::DockBuilderGetNode(dockspace_id) == 0)
	{
		ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
		ImGuiID test = ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace); // Add empty node
		ImGui::DockBuilderSetNodeSize(dockspace_id, ImVec2(width, height));
		ImGuiID dock_main_id = dockspace_id;
		ImGuiID dock_up_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.12f, 0, &dock_main_id);
		ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.25f, 0, &dock_main_id);
		ImGuiID dock_right_down_id = ImGui::DockBuilderSplitNode(dock_right_id, ImGuiDir_Right, 0.25f, 0, &dock_right_id);
		ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.2f, 0, &dock_main_id);
		ImGuiID dock_center_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 1.0f, 0, &dock_left_id);
		ImGuiID dock_down_id = ImGui::DockBuilderSplitNode(dock_center_id, ImGuiDir_Down, 0.2f, 0, &dock_center_id);
	}

	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

	DrawGUIHelper();

	ImGui::End();
}

void EditorGUI::DrawFrame()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
