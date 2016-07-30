/*
 * LibrePCB - Professional EDA for everyone!
 * Copyright (C) 2013 LibrePCB Developers, see AUTHORS.md for contributors.
 * http://librepcb.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBREPCB_FAVORITEPROJECTSMODEL_H
#define LIBREPCB_FAVORITEPROJECTSMODEL_H

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include <librepcbcommon/fileio/filepath.h>

/*****************************************************************************************
 *  Namespace / Forward Declarations
 ****************************************************************************************/
namespace librepcb {

class SmartTextFile;

namespace workspace {

class Workspace;

/*****************************************************************************************
 *  Class FavoriteProjectsModel
 ****************************************************************************************/

/**
 * @brief The FavoriteProjectsModel class
 */
class FavoriteProjectsModel : public QAbstractListModel
{
        Q_OBJECT

    public:

        // Constructors / Destructor
        FavoriteProjectsModel() = delete;
        FavoriteProjectsModel(const FavoriteProjectsModel& other) = delete;
        explicit FavoriteProjectsModel(const Workspace& workspace) noexcept;
        ~FavoriteProjectsModel() noexcept;

        // General Methods
        bool isFavoriteProject(const FilePath& filepath) const noexcept;
        void addFavoriteProject(const FilePath& filepath) noexcept;
        void removeFavoriteProject(const FilePath& filepath) noexcept;

        // Operator Overloadings
        FavoriteProjectsModel& operator=(const FavoriteProjectsModel& rhs) = delete;


    private:

        // General Methods
        void save() noexcept;

        // Inherited Methods
        int rowCount(const QModelIndex& parent = QModelIndex()) const;
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

        // Attributes
        const Workspace& mWorkspace;
        QScopedPointer<SmartTextFile> mFile;
        QList<FilePath> mFavoriteProjects;
};

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace workspace
} // namespace librepcb

#endif // LIBREPCB_FAVORITEPROJECTSMODEL_H
